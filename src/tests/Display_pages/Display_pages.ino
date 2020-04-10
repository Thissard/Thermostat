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

int MACHINE_STATE = SPLASH_SCREEN;
CONFIG settings;

void setup()
{
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
int dummyNavigationIndex = 0;
int dummyDayIndex = 0;
int dummyTimeIndex = 0;
int dummyTemperatureIndex = 0;

void loop()
{
  int t = millis() / 100;
  float te = t / 10;
  MACHINE_STATE = PROGRAMMING_SCREEN;
  switch (MACHINE_STATE)
  {
  case SPLASH_SCREEN: //SPLASH SCREEN SHOW
    Serial.println("[UPDATE] Display HomeScreen");
    disp.clearScreen();
    disp.showSplashScreen("0.0.0");
    delay(2000);
    disp.clearScreen();
    MACHINE_STATE = MAIN_SCREEN;
    ms = millis();
    Serial.print(settings.chrono.calendar.GIO[0]);
    Serial.print(settings.chrono.calendar.GIO[1]);
    Serial.print(settings.chrono.calendar.GIO[2]);
    break;
  case MAIN_SCREEN:
    disp.showMainScreen(99.9, te, 3, "127.0.0.1", settings);
    if (millis() - ms > 10000)
    {

      MACHINE_STATE = MENU_SCREEN;
      disp.clearScreen();
      ms = millis();
    }
    break;
  case MENU_SCREEN:
    disp.showMenuScreen(i);
    if (millis() - ms > 1500)
    {
      if (i == PROGRAMMING)
      {
        disp.clearScreen();
        MACHINE_STATE = PROGRAMMING_SCREEN;
      }
      if (i == TEMPERATURES)
      {
        disp.clearScreen();
        MACHINE_STATE = TEMPERATURE_SCREEN;
      }
      if (i == BRIGHTNESS)
      {
        disp.clearScreen();
        MACHINE_STATE = BRIGHTNESS_SCREEN;
      }
      i++;
      if (i > BRIGHTNESS)
      {
        i = PROGRAMMING;
        disp.clearScreen();
        MACHINE_STATE = MAIN_SCREEN;
      }
      ms = millis();
    }
    break;
  case BRIGHTNESS_SCREEN:
    delay(500);
    if (bright <= 0)
      fadeAmount = 20;
    if (bright >= 100)
      fadeAmount = -20;

    bright = bright + fadeAmount;

    disp.showBrightness(bright);
    //disp.setBacklight(bright);
    analogWrite(TFT_LED, bright * 1024 / 100);

    if (millis() - ms > 4000)
    {
      disp.clearScreen();
      ms = millis();
      MACHINE_STATE = MENU_SCREEN;
    }
    break;
  case PROGRAMMING_SCREEN:
    delay(500);
    dummyNavigationIndex++;
    if (dummyNavigationIndex > 2)
      dummyNavigationIndex = 0;
    dummyDayIndex++;
    if (dummyDayIndex > 7)
      dummyDayIndex = 0;
    dummyTimeIndex++;
    if (dummyTimeIndex > 23)
      dummyTimeIndex = 0;
    dummyTemperatureIndex++;
    if (dummyTemperatureIndex > 4)
      dummyTemperatureIndex = 0;
    disp.showProgrammation(dummyNavigationIndex, dummyDayIndex, dummyTimeIndex, dummyTemperatureIndex, settings);
    if (millis() - ms > 10000)
    {
      disp.clearScreen();
      ms = millis();
      MACHINE_STATE = MENU_SCREEN;
    }
    break;
  case TEMPERATURE_SCREEN:
    delay(500);
    if (millis() - ms > 4000)
    {
      ms = millis();
      disp.clearScreen();
      MACHINE_STATE = MENU_SCREEN;
    }
    break;
  }
}
void dummyInitialization()
{
  settings.chrono.setpoints.eco = 18.0;
  settings.chrono.setpoints.normal = 20;
  settings.chrono.setpoints.comfort = 22;
  settings.chrono.setpoints.comfort_p = 23;

  settings.chrono.calendar.DOM[0] = 1;
  settings.chrono.calendar.DOM[1] = 0;
  settings.chrono.calendar.DOM[2] = 2;
  settings.chrono.calendar.DOM[3] = 3;
  settings.chrono.calendar.DOM[4] = 0;
  settings.chrono.calendar.DOM[5] = 4;
  settings.chrono.calendar.DOM[6] = 4;
  settings.chrono.calendar.DOM[7] = 3;
  settings.chrono.calendar.DOM[8] = 2;
  settings.chrono.calendar.DOM[9] = 2;
  settings.chrono.calendar.DOM[10] = 1;
  settings.chrono.calendar.DOM[11] = 1;
  settings.chrono.calendar.DOM[12] = 0;
  settings.chrono.calendar.DOM[13] = 0;
  settings.chrono.calendar.DOM[14] = 1;
  settings.chrono.calendar.DOM[15] = 1;
  settings.chrono.calendar.DOM[16] = 0;
  settings.chrono.calendar.DOM[17] = 0;
  settings.chrono.calendar.DOM[18] = 0;
  settings.chrono.calendar.DOM[19] = 0;
  settings.chrono.calendar.DOM[20] = 0;
  settings.chrono.calendar.DOM[21] = 0;
  settings.chrono.calendar.DOM[22] = 0;
  settings.chrono.calendar.DOM[23] = 0;

  settings.chrono.calendar.LUN[0] = 1;
  settings.chrono.calendar.LUN[1] = 0;
  settings.chrono.calendar.LUN[2] = 2;
  settings.chrono.calendar.LUN[3] = 3;
  settings.chrono.calendar.LUN[4] = 0;
  settings.chrono.calendar.LUN[5] = 4;
  settings.chrono.calendar.LUN[6] = 4;
  settings.chrono.calendar.LUN[7] = 3;
  settings.chrono.calendar.LUN[8] = 2;
  settings.chrono.calendar.LUN[9] = 2;
  settings.chrono.calendar.LUN[10] = 1;
  settings.chrono.calendar.LUN[11] = 1;
  settings.chrono.calendar.LUN[12] = 0;
  settings.chrono.calendar.LUN[13] = 0;
  settings.chrono.calendar.LUN[14] = 1;
  settings.chrono.calendar.LUN[15] = 1;
  settings.chrono.calendar.LUN[16] = 0;
  settings.chrono.calendar.LUN[17] = 0;
  settings.chrono.calendar.LUN[18] = 0;
  settings.chrono.calendar.LUN[19] = 0;
  settings.chrono.calendar.LUN[20] = 0;
  settings.chrono.calendar.LUN[21] = 0;
  settings.chrono.calendar.LUN[22] = 0;
  settings.chrono.calendar.LUN[23] = 0;

  settings.chrono.calendar.MAR[0] = 1;
  settings.chrono.calendar.MAR[1] = 0;
  settings.chrono.calendar.MAR[2] = 2;
  settings.chrono.calendar.MAR[3] = 3;
  settings.chrono.calendar.MAR[4] = 0;
  settings.chrono.calendar.MAR[5] = 4;
  settings.chrono.calendar.MAR[6] = 4;
  settings.chrono.calendar.MAR[7] = 3;
  settings.chrono.calendar.MAR[8] = 2;
  settings.chrono.calendar.MAR[9] = 2;
  settings.chrono.calendar.MAR[10] = 1;
  settings.chrono.calendar.MAR[11] = 1;
  settings.chrono.calendar.MAR[12] = 0;
  settings.chrono.calendar.MAR[13] = 0;
  settings.chrono.calendar.MAR[14] = 1;
  settings.chrono.calendar.MAR[15] = 1;
  settings.chrono.calendar.MAR[16] = 0;
  settings.chrono.calendar.MAR[17] = 0;
  settings.chrono.calendar.MAR[18] = 0;
  settings.chrono.calendar.MAR[19] = 0;
  settings.chrono.calendar.MAR[20] = 0;
  settings.chrono.calendar.MAR[21] = 0;
  settings.chrono.calendar.MAR[22] = 0;
  settings.chrono.calendar.MAR[23] = 0;

  settings.chrono.calendar.MER[0] = 1;
  settings.chrono.calendar.MER[1] = 0;
  settings.chrono.calendar.MER[2] = 2;
  settings.chrono.calendar.MER[3] = 3;
  settings.chrono.calendar.MER[4] = 0;
  settings.chrono.calendar.MER[5] = 4;
  settings.chrono.calendar.MER[6] = 4;
  settings.chrono.calendar.MER[7] = 3;
  settings.chrono.calendar.MER[8] = 2;
  settings.chrono.calendar.MER[9] = 2;
  settings.chrono.calendar.MER[10] = 1;
  settings.chrono.calendar.MER[11] = 1;
  settings.chrono.calendar.MER[12] = 0;
  settings.chrono.calendar.MER[13] = 0;
  settings.chrono.calendar.MER[14] = 1;
  settings.chrono.calendar.MER[15] = 1;
  settings.chrono.calendar.MER[16] = 0;
  settings.chrono.calendar.MER[17] = 0;
  settings.chrono.calendar.MER[18] = 0;
  settings.chrono.calendar.MER[19] = 0;
  settings.chrono.calendar.MER[20] = 0;
  settings.chrono.calendar.MER[21] = 0;
  settings.chrono.calendar.MER[22] = 0;
  settings.chrono.calendar.MER[23] = 0;

  settings.chrono.calendar.GIO[0] = 1;
  settings.chrono.calendar.GIO[1] = 0;
  settings.chrono.calendar.GIO[2] = 2;
  settings.chrono.calendar.GIO[3] = 3;
  settings.chrono.calendar.GIO[4] = 0;
  settings.chrono.calendar.GIO[5] = 4;
  settings.chrono.calendar.GIO[6] = 4;
  settings.chrono.calendar.GIO[7] = 3;
  settings.chrono.calendar.GIO[8] = 2;
  settings.chrono.calendar.GIO[9] = 2;
  settings.chrono.calendar.GIO[10] = 1;
  settings.chrono.calendar.GIO[11] = 1;
  settings.chrono.calendar.GIO[12] = 0;
  settings.chrono.calendar.GIO[13] = 0;
  settings.chrono.calendar.GIO[14] = 1;
  settings.chrono.calendar.GIO[15] = 1;
  settings.chrono.calendar.GIO[16] = 0;
  settings.chrono.calendar.GIO[17] = 0;
  settings.chrono.calendar.GIO[18] = 0;
  settings.chrono.calendar.GIO[19] = 0;
  settings.chrono.calendar.GIO[20] = 0;
  settings.chrono.calendar.GIO[21] = 0;
  settings.chrono.calendar.GIO[22] = 0;
  settings.chrono.calendar.GIO[23] = 0;

  settings.chrono.calendar.VEN[0] = 1;
  settings.chrono.calendar.VEN[1] = 0;
  settings.chrono.calendar.VEN[2] = 2;
  settings.chrono.calendar.VEN[3] = 3;
  settings.chrono.calendar.VEN[4] = 0;
  settings.chrono.calendar.VEN[5] = 4;
  settings.chrono.calendar.VEN[6] = 4;
  settings.chrono.calendar.VEN[7] = 3;
  settings.chrono.calendar.VEN[8] = 2;
  settings.chrono.calendar.VEN[9] = 2;
  settings.chrono.calendar.VEN[10] = 1;
  settings.chrono.calendar.VEN[11] = 1;
  settings.chrono.calendar.VEN[12] = 0;
  settings.chrono.calendar.VEN[13] = 0;
  settings.chrono.calendar.VEN[14] = 1;
  settings.chrono.calendar.VEN[15] = 1;
  settings.chrono.calendar.VEN[16] = 0;
  settings.chrono.calendar.VEN[17] = 0;
  settings.chrono.calendar.VEN[18] = 0;
  settings.chrono.calendar.VEN[19] = 0;
  settings.chrono.calendar.VEN[20] = 0;
  settings.chrono.calendar.VEN[21] = 0;
  settings.chrono.calendar.VEN[22] = 0;
  settings.chrono.calendar.VEN[23] = 0;

  settings.chrono.calendar.SAB[0] = 1;
  settings.chrono.calendar.SAB[1] = 0;
  settings.chrono.calendar.SAB[2] = 2;
  settings.chrono.calendar.SAB[3] = 3;
  settings.chrono.calendar.SAB[4] = 0;
  settings.chrono.calendar.SAB[5] = 4;
  settings.chrono.calendar.SAB[6] = 4;
  settings.chrono.calendar.SAB[7] = 3;
  settings.chrono.calendar.SAB[8] = 2;
  settings.chrono.calendar.SAB[9] = 2;
  settings.chrono.calendar.SAB[10] = 1;
  settings.chrono.calendar.SAB[11] = 1;
  settings.chrono.calendar.SAB[12] = 0;
  settings.chrono.calendar.SAB[13] = 0;
  settings.chrono.calendar.SAB[14] = 1;
  settings.chrono.calendar.SAB[15] = 1;
  settings.chrono.calendar.SAB[16] = 0;
  settings.chrono.calendar.SAB[17] = 0;
  settings.chrono.calendar.SAB[18] = 0;
  settings.chrono.calendar.SAB[19] = 0;
  settings.chrono.calendar.SAB[20] = 0;
  settings.chrono.calendar.SAB[21] = 0;
  settings.chrono.calendar.SAB[22] = 0;
  settings.chrono.calendar.SAB[23] = 0;
}
