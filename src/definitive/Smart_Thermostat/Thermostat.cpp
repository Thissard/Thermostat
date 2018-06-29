#include "Arduino.h"
#include "Thermostat.h"

Thermostat::Thermostat(unsigned char control_pin){
  _control_pin = control_pin;
  pinMode(_control_pin, OUTPUT);
  digitalWrite(_control_pin, LOW);
  _heating = false;
  _mode = AUTOMATIC_MODE;
}

Thermostat::Thermostat(unsigned char control_pin, unsigned char mode){
  _control_pin = control_pin;
  digitalWrite(_control_pin, LOW);
  _heating = false;
  _mode = mode;
}

void Thermostat::set_mode(unsigned char mode){
  _mode = mode;
  if (_mode == MANUAL_MODE)
    turn_heater_OFF();
}

void Thermostat::turn_heater_ON(void){
  digitalWrite(_control_pin, HIGH);
  _heating = true;
}

void Thermostat::turn_heater_OFF(void){
  digitalWrite(_control_pin, LOW);
  _heating = false;
}

bool Thermostat::get_heater_state(void){
  return _heating;
}

void Thermostat::update(float room_temperature, float set_point){
  if (_mode == AUTOMATIC_MODE)
    if (room_temperature < set_point){
      turn_heater_ON();
    }
    else{
      turn_heater_OFF();
    }
}
