#include "ConfigParser.h"
#include "Structures.h"

ConfigParser::ConfigParser(String fileName){
  this->_filename = fileName;
}

String ConfigParser::loadConfiguration(void){
  SPIFFS.begin();
  File configFile = SPIFFS.open(this->_filename, FILE_READ);

  const size_t capacity = 7*JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 330;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, configFile);
  const char* temp_string;
  JsonObject network = doc["network"];
  temp_string = network["net-name"]; // "TIM-90267905"
  config.network.SSID = temp_string;
  temp_string = network["net-pass"]; // "ufSzgF1bBGDlMWhhhG7OYrTC"
  config.network.password = temp_string;
  temp_string = network["ip"]; // "192.168.1.222"
  config.network.ip = temp_string;
  temp_string = network["sub"]; // "255.255.255.0"
  config.network.subnet = temp_string;
  temp_string = network["dns"]; // "192.168.1.1"
  config.network.dns = temp_string;
  temp_string = network["gateway"]; // "192.168.1.1"
  config.network.gateway = temp_string;
  temp_string = network["ntp-server"]; // "0.it.pool.ntp.org"
  config.network.NTPServerName = temp_string;
  
  JsonObject setpoints = doc["setpoints"];
  config.chrono.setpoints.eco = setpoints["eco"]; // 18
  config.chrono.setpoints.normal = setpoints["normal"]; // 21
  config.chrono.setpoints.comfort = setpoints["comfort"]; // 23
  config.chrono.setpoints.comfort_p = setpoints["comfort+"]; // 25
  
  JsonObject chrono = doc["chrono"];
  JsonArray chrono_LUN = chrono["LUN"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.LUN[i] = chrono_LUN[i];
  }
  JsonArray chrono_MAR = chrono["MAR"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.MAR[i] = chrono_MAR[i];
  }
  JsonArray chrono_MER = chrono["MER"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.MER[i] = chrono_MER[i];
  }
  JsonArray chrono_GIO = chrono["GIO"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.GIO[i] = chrono_GIO[i];
  }
  JsonArray chrono_VEN = chrono["VEN"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.GIO[i] = chrono_VEN[i];
  }
  JsonArray chrono_SAB = chrono["SAB"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.SAB[i] = chrono_SAB[i];
  }
  JsonArray chrono_DOM = chrono["DOM"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.DOM[i] = chrono_DOM[i];
  }
  
  int display_bright = doc["display"]["bright"]; // 100
  
  configFile.close();
  SPIFFS.end();
return "";

}

String ConfigParser::saveConfiguration(void){
  SPIFFS.begin();
  SPIFFS.remove(this->_filename);
  
  const size_t capacity = 7*JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 330;
  DynamicJsonDocument doc(capacity);
  File configFile = SPIFFS.open(this->_filename, FILE_WRITE);
  
  JsonObject network = doc.createNestedObject("network"); 

  network["net-name"] = config.network.SSID;
  network["net-pass"] = config.network.password;
  network["ip"] = config.network.ip;
  network["sub"] = config.network.subnet;
  network["dns"] = config.network.dns;
  network["gateway"] = config.network.gateway;
  network["ntp-server"] = config.network.NTPServerName;
  
  JsonObject setpoints = doc.createNestedObject("setpoints");
  setpoints["eco"] = config.chrono.setpoints.eco;
  setpoints["normal"] = config.chrono.setpoints.normal; 
  setpoints["comfort"] = config.chrono.setpoints.comfort; 
  setpoints["comfort+"] = config.chrono.setpoints.comfort_p;
  
  JsonObject chrono = doc.createNestedObject("chrono");
  JsonArray chrono_LUN = chrono.createNestedArray("LUN");
  for (int i = 0; i < 24; i++)
  {
    chrono_LUN.add(config.chrono.calendar.LUN[i]);
  }
  JsonArray chrono_MAR = chrono.createNestedArray("MAR");
  for (int i = 0; i < 24; i++)
  {
    chrono_MAR.add(config.chrono.calendar.MAR[i]);
  }
  JsonArray chrono_MER = chrono.createNestedArray("MER");
  for (int i = 0; i < 24; i++)
  {
    chrono_MER.add(config.chrono.calendar.MER[i]);
  }
  JsonArray chrono_GIO = chrono.createNestedArray("GIO");
  for (int i = 0; i < 24; i++)
  {
    chrono_GIO.add(config.chrono.calendar.GIO[i]);
  }
  JsonArray chrono_VEN = chrono.createNestedArray("VEN");
  for (int i = 0; i < 24; i++)
  {
    chrono_VEN.add(config.chrono.calendar.VEN[i]);
  }
  JsonArray chrono_SAB = chrono.createNestedArray("SAB");
  for (int i = 0; i < 24; i++)
  {
    chrono_SAB.add(config.chrono.calendar.SAB[i]);
  }
  JsonArray chrono_DOM = chrono.createNestedArray("DOM");
  for (int i = 0; i < 24; i++)
  {
    chrono_DOM.add(config.chrono.calendar.DOM[i]);
  }
  JsonObject display = doc.createNestedObject("display");
  display["bright"] = 100;
  serializeJson(doc, configFile);
 return ""; 
}
