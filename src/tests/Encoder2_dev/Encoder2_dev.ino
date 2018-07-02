#include "UserCommands.h"

#define IN_ENC_A D1
#define IN_ENC_B D3
#define IN_ENC_BUTTON D6

UserCommands encoder(IN_ENC_A, IN_ENC_B, IN_ENC_BUTTON);

// pin assignments

#define BUTTON_PIN D6
#define LED_PIN LED_BUILTIN

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

  if (encoder.buttonWasPressed()){
    Serial.println("BUTTON PRESSED");
  }

}
