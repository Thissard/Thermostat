//////////////////////////////////////
//DISPLAY
//////////////////////////////////////
#include "Display.h"

#define TFT_SCLK D5
#define TFT_DATA D7
#define TFT_CD D4
#define TFT_CS D8
#define TFT_LED RX

Display disp = Display(TFT_LED, TFT_CS, TFT_CD);

uint16_t colors[24] = {
    ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, // 00 01 02 03
    ILI9341_BLUE, ILI9341_BLUE, ILI9341_GREEN, ILI9341_GREEN, // 04 05 06 07
    ILI9341_GREEN, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, // 08 09 10 11
    ILI9341_BLUE, 0x2924, 0x2924, ILI9341_BLUE, // 12 13 14 15
    ILI9341_ORANGE, ILI9341_ORANGE, ILI9341_ORANGE, ILI9341_ORANGE, // 16 17 18 19
    ILI9341_RED, ILI9341_RED, ILI9341_GREEN, ILI9341_GREEN // 20 21 22 23
    };

int MACHINE_STATE = 0;

void setup() {
  Serial.begin(9600);
  delay(50);
  disp.begin();

  pinMode(TFT_LED, OUTPUT);
  analogWrite(TFT_LED, 1024);
}

int i = 0;
int ms = 0;
int bright = 0;
int fadeAmount = 20;

void loop(){
  int t = millis()/100;
  float te = t/10;
  switch (MACHINE_STATE){
    case 0: //SPLASH SCREEN SHOW
      Serial.println("[UPDATE] Display HomeScreen");
      disp.clearScreen();
      disp.showSplashScreen("0.0.0");
      delay(2000);
      disp.clearScreen();
      MACHINE_STATE=10;
      ms = millis();
      break;
    case 10:
      disp.showMainScreen(99.9 , te, 3, "127.0.0.1", colors);
      if (millis() - ms > 10000){
        MACHINE_STATE = 20;
        disp.clearScreen();
        ms= millis();
      }
      break;
    case 20: 
      disp.showMenuScreen(i);
      if (millis() - ms > 1500){
        if (i==1){
        disp.clearScreen();
        MACHINE_STATE = 30;
        }
        i++;
        if (i>2){
          i = 0;
          disp.clearScreen();
          MACHINE_STATE = 10;
        }
       ms = millis(); 
      }
    break;
    case 30:
      delay (500);
      if (bright <=0)
        fadeAmount = 20;
      if (bright >=100)
        fadeAmount = -20;
        
      bright = bright + fadeAmount;
      
      disp.showBrightness(bright);
      //disp.setBacklight(bright);
      analogWrite(TFT_LED,bright*1024/100);
      
      
      if (millis() - ms > 4000){
        disp.clearScreen();
        ms= millis();
        MACHINE_STATE = 20;
      }
    break;
  }
}


