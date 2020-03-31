#include "Structures.h"
#include "FS.h"
#include <ArduinoJson.h>

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
