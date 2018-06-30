#include "UserCommands.h"

#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
                           //switch is closed, this is negative logic, i.e. a high state
                           //means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20 //A debounce time of 20 milliseconds usually works well for tactile button switches.
int prev_time = 0;

UserCommands::UserCommands(uint8_t a_pin, uint8_t b_pin, uint8_t pushbutton_pin){
  _a = a_pin;
  _b = b_pin;
  _push = pushbutton_pin;
  _sensibility = 2;
}

void UserCommands::begin(void){
  _rotary = new ESPRotary(_a, _b);
  _button = new Button(_push, PULLUP, INVERT, DEBOUNCE_MS);

}

void UserCommands::update(void){
  _rotary->loop();
  _button->read();


  if (millis() - prev_time > 2000){ //timeout: no user action for too long time
    Serial.println("Reset everything!");
    _rotary->resetPosition();
    _turnedRight = false;
    _turnedLeft = false;
    prev_time=millis();
  }
  
  if ( _rotary->getDirection() == RE_RIGHT && _rotary->getPosition() > _sensibility ){
    //RIGHT COMMAND
    _rotary->resetPosition();
    _turnedRight = true;
    _turnedLeft = false;
    prev_time=millis();
  }
  
  if ( _rotary->getDirection() == RE_LEFT && _rotary->getPosition() < - _sensibility ){
    //LEFT COMMAND
    _rotary->resetPosition();
    _turnedLeft = true;
    _turnedRight = false;
    prev_time=millis();
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


