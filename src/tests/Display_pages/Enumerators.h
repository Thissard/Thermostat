#ifndef ENUMERATORS
#define ENUMERATORS

enum DAYS
{
  NO_DAY,
  DOMENICA,
  LUNEDI,
  MARTEDI,
  MERCOLEDI,
  GIOVEDI,
  VENERDI,
  SABATO
};

enum PROG_SETPOINTS
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
  CHRONO_SCREEN,
  SETPOINTS_SCREEN,
  BRIGHTNESS_SCREEN
};

enum MENU_INDEX
{
  PROG_CHRONO,
  PROG_SETPOINTS,
  BRIGHTNESS,
  MENU_BACK //LEAVE 'BACK' AS LAST
};

enum PROGRAMMING_CHRONO_INDEX
{
  CHOOSE_DAY,
  CHOOSE_TIME,
  CHOOSE_TEMPERATURE
};

enum PROGRAMMING_SETPOINT_INDEX
{
  CHOOSE_PROGRAM,
  CHOOSE_SETPOINT
};
#endif
