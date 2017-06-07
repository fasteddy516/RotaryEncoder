/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
Library written by Edward Wright (fasteddy@thewrightspace.net)
The encoder reading logic in the update() method is based on example code
written by Oleg Mazurov, available at the time this was written at:
https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Arduino.h"
#include "RotaryEncoder.h"

// default (and only) constructor
RotaryEncoder::RotaryEncoder() {
  idlesHigh();
  debounceTime_ms = DEFAULT_DEBOUNCE_MS;
  reset();  
}

// assigns the specified pins as encoder channel a/b inputs
void RotaryEncoder::attach(byte encoderChannelAPin, byte encoderChannelBPin) {
  channelAPin = encoderChannelAPin;
  channelBPin = encoderChannelBPin;
}

// indicates that channel a/b input signals are high when encoder is not moving
void RotaryEncoder::idlesHigh() {
  initialState = INITIALSTATE_HIGHIDLE;
}

// indicates that channel a/b input signals are low when encoder is not moving
void RotaryEncoder::idlesLow() {
  initialState = INITIALSTATE_LOWIDLE;
}

// sets the debounce (rate limiter) duration used for reading the encoder
void RotaryEncoder::debounceTime(unsigned int debounce_ms) {
  debounceTime_ms = debounce_ms;
}

// reads the current state of the encoder and increments/decrements the virtual
// position value.  Returns true if the encoder is being rotated, false if it is not.
// The core encoder reading logic in this method is based on Oleg Mazurov's example code
// posted at https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/.
bool RotaryEncoder::update() {
  
  // credit goes to Oleg Mazurov for the next four lines...
  const char stateTable[] = { 0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0 }; // direction lookup table
  encoderState <<= 2; // shift encoder state over two bits to make room for new data
  encoderState |= (((digitalRead(channelAPin) << 1) | digitalRead(channelBPin)) & 0x03);  // get current state
  currentDirection = stateTable[(encoderState & 0x0F)]; //determine current direction

  // if debounce (rate limiter) is active and the debounce time has elapsed, end the debounce
  if (debouncing && ((millis() - previousMillis_ms) >= debounceTime_ms)) {
    debouncing = false;
  }

  // if a change has occurred and we are not currently debouncing
  if (currentDirection && !debouncing) {
    debouncing = true; // enable debouncer
    if (currentDirection == DIRECTION_UP && currentPosition < maxPosition) {
      currentPosition += currentDirection; //increment virtual position if counting up
    }
    else if (currentDirection == DIRECTION_DOWN && currentPosition > minPosition) {
      currentPosition += currentDirection; //decrement virtual position if counting down
    }
    previousMillis_ms = millis(); // store timestamp for last encoder read
    return true; // indicate that encoder is in motion
  }
  else {
    return false; // indicate that encoder is idle or being debounced
  }
}

// returns the direction the encoder was moving on the last update() call
char RotaryEncoder::direction() {
  return currentDirection;
}

// returns true if the virtual position increased
bool RotaryEncoder::increased() {
  return (currentDirection == DIRECTION_UP);
}

// returns true if the virtual position decreased
bool RotaryEncoder::decreased() {
  return (currentDirection == DIRECTION_DOWN);
}

// sets the minimum virtual position value and updates current position if necessary
void RotaryEncoder::positionMin(int minValue) {
  minPosition = minValue;
  if (currentPosition < minPosition) {
    currentPosition = minPosition;
  }
}

// sets the maximum virtual position value and updates current position if necessary
void RotaryEncoder::positionMax(int maxValue) {
  maxPosition = maxValue;
  if (currentPosition > maxValue) {
    currentPosition = maxValue;
  }
}

// sets the current virtual position to the specified value if it is in range
void RotaryEncoder::setPosition(int newPosition) {
  if ((newPosition >= minPosition) && (newPosition <= maxPosition)) {
    currentPosition = newPosition;
  }
}

// returns the current virtual position of the encoder
int RotaryEncoder::getPosition() {
  return currentPosition;
}

// resets the stored encoder start to startup values
void RotaryEncoder::reset() {
  encoderState = initialState;
  debouncing = false;
  currentDirection = DIRECTION_NONE;
  currentPosition = minPosition;
}

// sets the debounce timestamp to the current value of millis()
void RotaryEncoder::retime() {
  previousMillis_ms = millis();
}