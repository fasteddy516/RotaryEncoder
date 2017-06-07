/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
Library written by Edward Wright (fasteddy@thewrightspace.net)
The encoder reading logic in the update() method is based on example code
written by Oleg Mazurov, available at the time this was written at:
https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

// these are the default settings used in the constructor - all of them can be changed after instantiation using the corresponding methods 
#define DEFAULT_DEBOUNCE_MS   85
#define DEFAULT_POSITIONMIN   0
#define DEFAULT_POSITIONMAX   255

// these are defined purely for code readability, and should not be modified
#define INITIALSTATE_HIGHIDLE 0x03
#define INITIALSTATE_LOWIDLE  0x00
#define DIRECTION_NONE        0
#define DIRECTION_DOWN        -1
#define DIRECTION_UP          1


class RotaryEncoder {
  public:
    // public methods...
    RotaryEncoder(); // default constructor
    void attach(byte encoderChannelAPin, byte encoderChannelBPin); // assigns the specified pins as encoder channel a/b inputs
    void idlesHigh(); // indicates that channel a/b input signals are high when encoder is not moving
    void idlesLow(); // indicates that channel a/b input signals are low when encoder is not moving
    void debounceTime(unsigned int debounce_ms); // sets the debounce (rate limiter) duration used for reading the encoder
    bool update(); // reads the current state of the encoder
    char direction(); // returns the direction the encoder was moving on the last update() call
    bool increased(); // returns true if the virtual position increased
    bool decreased(); // returns true if the virtual position decreased
    void positionMin(int minValue); // sets the minimum virtual position value and updates current position if necessary
    void positionMax(int maxValue); // sets the maximum virtual position value and updates current position if necessary
    void setPosition(int newPosition); // sets the current virtual position to the specified value if it is in range
    int getPosition(); // returns the current virtual position of the encoder
    void reset(); // resets the stored encoder start to startup values
    void retime(); // sets the debounce timestamp to the current value of millis()

  private:
    // private instance variables...
    byte channelAPin; // arduino pin that encoder channel 'a' is connected to
    byte channelBPin; // arduino pin that encoder channel 'b' is connected to
    byte encoderState; // remember the state of the encoder channel a/b signals
    byte initialState; // initial state at which encoder should be when idle

    unsigned long previousMillis_ms; // remember when encoder was last read
    unsigned int debounceTime_ms; // debounce (rate limiting) value to limit encoder read frequency
    bool debouncing; // flag to indicate if encoder read should be blocked or not
    
    int minPosition; // the minimum virtual position value
    int maxPosition; // the maximum virtual position value
    int currentPosition; // the current virtual position
    char currentDirection; // the current direction based on encoder rotation
};

#endif

