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

int MACHINE_STATE = 0;
CONFIG settings;

void setup() {
  Serial.begin(9600);
  delay(50);
  disp.begin();

  pinMode(TFT_LED, OUTPUT);
  analogWrite(TFT_LED, 1024);

  dummyInitialization();
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
      Serial.print(settings.chrono.calendar.GIO[0]);
      Serial.print(settings.chrono.calendar.GIO[1]);
      Serial.print(settings.chrono.calendar.GIO[2]);
      break;
    case 10:
      disp.showMainScreen(99.9 , te, 3, "127.0.0.1", settings );
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

void dummyInitialization(){
      settings.chrono.setpoints.eco = 18.0;
      settings.chrono.setpoints.normal = 20;
      settings.chrono.setpoints.comfort = 22;
      settings.chrono.setpoints.comfort_p = 23;

      settings.chrono.calendar.DOM[0]=1;
      settings.chrono.calendar.DOM[1]=0;
      settings.chrono.calendar.DOM[2]=2;
      settings.chrono.calendar.DOM[3]=3;
      settings.chrono.calendar.DOM[4]=0;
      settings.chrono.calendar.DOM[5]=4;
      settings.chrono.calendar.DOM[6]=4;
      settings.chrono.calendar.DOM[7]=3;
      settings.chrono.calendar.DOM[8]=2;
      settings.chrono.calendar.DOM[9]=2;
      settings.chrono.calendar.DOM[10]=1;
      settings.chrono.calendar.DOM[11]=1;
      settings.chrono.calendar.DOM[12]=0;
      settings.chrono.calendar.DOM[13]=0;
      settings.chrono.calendar.DOM[14]=1;
      settings.chrono.calendar.DOM[15]=1;
      settings.chrono.calendar.DOM[16]=0;
      settings.chrono.calendar.DOM[17]=0;
      settings.chrono.calendar.DOM[18]=0;
      settings.chrono.calendar.DOM[19]=0;
      settings.chrono.calendar.DOM[20]=0;
      settings.chrono.calendar.DOM[21]=0;
      settings.chrono.calendar.DOM[22]=0;
      settings.chrono.calendar.DOM[23]=0;

      *settings.chrono.calendar.LUN = *settings.chrono.calendar.DOM;
      *settings.chrono.calendar.MAR= *settings.chrono.calendar.DOM;
      *settings.chrono.calendar.MER= *settings.chrono.calendar.DOM;
      *settings.chrono.calendar.GIO= *settings.chrono.calendar.DOM;
      *settings.chrono.calendar.VEN= *settings.chrono.calendar.DOM;
      *settings.chrono.calendar.SAB= *settings.chrono.calendar.DOM;

      
  }
