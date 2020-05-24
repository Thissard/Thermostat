#include "WString.h"

#ifndef STRUCTURES
  #define STRUCTURES
  
  struct NETWORK{
    String SSID;
    String password;
    String ip; //dummy
    String subnet; //dummy
    String gateway; //dummy
    String dns; //dummy
    String NTPServerName;
  };
  struct SETPOINTS {
      double eco;
      double normal;
      double comfort;
      double comfort_p;
  };
  struct CALENDAR {
      int LUN[24];
      int MAR[24];
      int MER[24];
      int GIO[24];
      int VEN[24];
      int SAB[24];
      int DOM[24];
  };
  struct CHRONO {
      SETPOINTS setpoints;
      CALENDAR calendar;
  };
  struct THERMOSTAT{
    int brightness;
  };
  struct CONFIG {
      THERMOSTAT thermostat;
      NETWORK network;
      CHRONO chrono;
  };

  struct SQUARE_COLOR{
    uint16_t OFF = 0x7BEF;         ///< 123, 125, 123
    uint16_t ECO = 0x001F;         ///<   0,   0, 255
    uint16_t NORMAL = 0x07E0;      ///<   0, 255,   0
    uint16_t COMFORT = 0xFD20;     ///< 255, 165,   0
    uint16_t COMFORT_PLUS = 0xF800;///< 255,   0,   0
  };
  
  extern SQUARE_COLOR square_colors;
  extern CONFIG settings;
#endif
