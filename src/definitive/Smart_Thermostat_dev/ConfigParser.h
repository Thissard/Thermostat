#include "FS.h"
#include <ArduinoJson.h>

struct THERMOSTAT{
  int brightness;
};

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
    NETWORK network;
    CHRONO chrono;
};

#define FILE_WRITE "a"
#define FILE_READ "r"

class ConfigParser{
  public:
    ConfigParser(String fileName);
    String loadConfiguration(void);
    String saveConfiguration(void);
    CONFIG config;
private:
    File _configFile;
    void _deserialize(DynamicJsonDocument doc);
    String _filename;
};
