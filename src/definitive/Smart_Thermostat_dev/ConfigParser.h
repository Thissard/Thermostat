#include "FS.h"
#include <ArduinoJson.h>

struct THERMOSTAT{
  int brightness;
};
/*
struct NETWORK{
    const char* SSID;
    const char* password;
    const char* ip; //dummy
    const char* subnet; //dummy
    const char* gateway; //dummy
    const char* dns; //dummy
    const char* NTPServerName;
};
*/
struct SETPOINTS {
    double eco;
    double normal;
    double comfort;
    double comfort_p;
};

struct CALENDAR {
    double LUN[24];
    double MAR[24];
    double MER[24];
    double GIO[24];
    double VEN[24];
    double SAB[24];
    double DOM[24];
};

struct CHRONO {
    SETPOINTS setpoints;
    CALENDAR calendar;
};

struct CONFIG {
    THERMOSTAT thermostat;
    //NETWORK network;
    CHRONO chrono;
};

class ConfigParser{
  public:
    String loadConfiguration(void);
    String saveConfiguration(void);
    CONFIG config;
private:
    File configFile;
    void deserialize(DynamicJsonDocument doc);
};
