// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

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

CONFIG config;
const char* temp_string;

void setup() {
  // Initialize Serial port
  loadJsonConfiguration();
}

void loop() {
  // not used in this example
  Serial.println("EVERYTHING FROM JSON IS: ");
  
  Serial.print("SSID: ");
  Serial.println(config.network.SSID);

  Serial.print("Password: ");
  Serial.println(config.network.password);
  
  Serial.print("IP: ");
  Serial.println(config.network.ip);

  Serial.print("Subnet: ");
  Serial.println(config.network.subnet);

  Serial.print("DNS: ");
  Serial.println(config.network.dns);

  delay(1000);
}

void loadJsonConfiguration(void){
  Serial.begin(9600);
  SPIFFS.begin();
  
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
  }
  
  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = 7*JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 330;
  DynamicJsonDocument doc(capacity);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configFile);
  
  size_t size = configFile.size();
  Serial.print("File size: ");
  Serial.println(size);

  Serial.print("File Content:");
  while (configFile.available()) {
    Serial.write(configFile.read());
  }
  Serial.println();
  
  //const char* json = "{\"network\":{\"net-name\":\"\\"TIM-90267905\\"\",\"net-pass\":\"\\"ufSzgF1bBGDlMWhhhG7OYrTC\\"\",\"ip\":\"\\"192.168.1.222\\"\",\"sub\":\"\\"255.255.255.0\\"\",\"dns\":\"\\"192.168.1.1\\"\",\"gateway\":\"\\"192.168.1.1\\"\",\"ntp-server\":\"\\"0.it.pool.ntp.org\\"\"},\"setpoints\":{\"eco\":18,\"normal\":21,\"comfort\":23,\"comfort+\":25},\"chrono\":{\"Lunedì\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18],\"Martedì\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18],\"Mercoledì\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18],\"Giovedì\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18],\"Venerdì\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18],\"Sabato\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18],\"Domenica\":[18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18]},\"display\":{\"bright\":100}}";
  if (error)
  Serial.println(F("Failed to read file, using default configuration"));
  Serial.println(error.c_str());
  
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
    config.chrono.setpoints.comfort= setpoints["comfort"]; // 23
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
}


// Visit https://arduinojson.org/v6/example/generator/ for more.
