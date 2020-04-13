#ifndef ENUMERATORS
#define ENUMERATORS

enum
{
  NONE,
  DOMENICA,
  LUNEDI,
  MARTEDI,
  MERCOLEDI,
  GIOVEDI,
  VENERDI,
  SABATO
};

enum
{
  OFF,
  ECO,
  NORMAL,
  COMFORT,
  COMFORT_PLUS
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MACHINE STATES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum SCREENS
{
  SPLASH_SCREEN,
  MAIN_SCREEN,
  MENU_SCREEN,
  PROGRAMMING_SCREEN,
  TEMPERATURE_SCREEN,
  BRIGHTNESS_SCREEN
};

enum MENU_INDEX
{
  PROGRAMMING,
  TEMPERATURES,
  BRIGHTNESS,
  BACK //LEAVE 'BACK' AS LAST
};

enum PROGRAMMING_INDEX
{
  CHOOSE_DAY,
  CHOOSE_TIME,
  CHOOSE_TEMPERATURE
};

#endif
