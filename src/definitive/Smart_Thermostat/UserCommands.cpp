#include "UserCommands.h"

int prev_time = 0;
UserCommands::UserCommands(uint8_t a_pin, uint8_t b_pin, uint8_t pushbutton_pin){
  _a = a_pin;
  _b = b_pin;
  _push = pushbutton_pin;
  _sensibility = 2;  
  
  _turnedRight = false;
  _turnedLeft = false;
  _buttonPressed = false;
  _buttonState = false;
  _bDebounce = 25;

  pinMode(_push, INPUT_PULLUP);
}

void UserCommands::begin(void){
  _rotary = new ESPRotary(_a, _b, 1);
  _buttonState = !digitalRead(_push);
}

void UserCommands::update(void){
  //TIMEOUT WHEN USER DONT DO ANY ACTION
  if (millis() - prev_time > 2000){ //timout occour after 2s
    //Serial.println("User Commands Timeout Occoured: Reset");
    _rotary->resetPosition();
    _turnedRight = false;
    _turnedLeft = false;
    _buttonPressed = false;
    prev_time=millis();
  }

  _rotary->loop();
  //RIGHT COMMAND DETECTION
  if ( _rotary->getDirection() == RE_RIGHT && _rotary->getPosition() > _sensibility ){
    _rotary->resetPosition();
    _turnedRight = true;
    _turnedLeft = false;
    prev_time=millis();
  }

  //LEFT COMMAND DETECTION
  if ( _rotary->getDirection() == RE_LEFT && _rotary->getPosition() < - _sensibility ){
    _rotary->resetPosition();
    _turnedLeft = true;
    _turnedRight = false;
    prev_time=millis();
  }

  //BUTTON PRESSED
  _buttonState = !digitalRead(_push);
  if (millis() - _bLastChange < _bDebounce){
    _bChanged = false;
  }
  else{
    _bChanged = (_buttonState != _buttonLastState);
    if (_bChanged)_bLastChange = millis();
    if (_bChanged && !_buttonState) _buttonPressed = true;
    _buttonLastState = _buttonState;
  }
  
}

bool UserCommands::turnedLeft(void){
  //when called reset status
  bool old_status = _turnedLeft;
  _turnedLeft = false;
  return old_status;
}

bool UserCommands::turnedRight(void){
  //when called reset status
  bool old_status = _turnedRight;
  _turnedRight = false;
  return old_status;
}

bool UserCommands::buttonWasPressed(void){
  //when called reset status
  bool old_status = _buttonPressed;
  _buttonPressed = false;
  return old_status;
}


