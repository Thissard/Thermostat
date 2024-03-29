#include <TimeLib.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/Gameplay20pt.h>
#include <Fonts/Classic8pt7b.h>
#include "Enumerators.h"
#include "Structures.h"

class Display
{
public:
  Display(int8_t led, int8_t cs, int8_t dc, int8_t sdi_mosi = -1, int8_t sck = -1);
  void clearScreen(void);

  void standby(void);
  void wake_up(void);

  void setBacklight(int backlight);

  void begin(void);

  void showSplashScreen(String project_version);
  void showMainScreen(float temperature, float humidity, uint8_t connectionON, String IP, CONFIG configuration);
  void showMenuScreen(int selection);
  void showProgrammation(int navigation_index, int day_index, int hour_index, int temperature_index, CONFIG configuration);
  void showTempSetpointsSettings(int navigation_index, int setpoint_index, float temperature, CONFIG configuration);
  void showBrightness(int selection);

private:
  int8_t _cs_pin;
  int8_t _dc_pin;
  int8_t _sdi_mosi_pin;
  int8_t _sck_pin;
  int8_t _led_pin;
  float _backlight;
  Adafruit_ILI9341 *tft;
};
