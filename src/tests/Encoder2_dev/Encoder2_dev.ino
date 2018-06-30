#include "UserCommands.h"

#define IN_ENC_A D1
#define IN_ENC_B D2
#define IN_ENC_BUTTON D7

UserCommands encoder(IN_ENC_A, IN_ENC_B, IN_ENC_BUTTON);

void setup() {
  Serial.begin(9600);
  delay(50);
  encoder.begin();
}

void loop() {

  encoder.update();
  
  if (encoder.turnedLeft()){
    Serial.println("LEFT COMMAND");
  }
  
  if (encoder.turnedRight()){
    Serial.println("RIGHT COMMAND");
  }
  
}
