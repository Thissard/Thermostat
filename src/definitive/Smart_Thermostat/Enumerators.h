#ifndef ENUMERATORS
  #define ENUMERATORS
  
   enum{
    NONE,
    DOMENICA,
    LUNEDI,
    MARTEDI,
    MERCOLEDI,
    GIOVEDI,
    VENERDI,
    SABATO
  };
  
  enum{
    OFF,
    ECO,
    NORMAL,
    COMFORT,
    COMFORT_PLUS
  };

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //MACHINE STATES
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  enum SCREENS{
    SPLASH_SCREEN,
    MAIN_SCREEN,
    MENU_SCREEN,
    BRIGHTNESS_SCREEN,
    CHRONO_SCREEN
  };
  
  enum MENU_INDEX{
    PROGRAMS,
    BRIGHTNESS,
    BACK  //LEAVE 'BACK' AS LAST
  };
  
#endif
