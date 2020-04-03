// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

#include "FS.h"
#include <ArduinoJson.h>

#define FILE_WRITE "a"
#define FILE_READ "r"

const char *filename = "/config.json";

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

struct CONFIG {
    THERMOSTAT thermostat;
    NETWORK network;
    CHRONO chrono;
};

CONFIG config;
const char* temp_string;

void setup() {
  Serial.begin(9600);
  //load datas from flash and store in configuration
  loadJsonConfiguration();
  printTestData();

  //change configuration by code and save new configuration to flash
  config.chrono.setpoints.eco += 13.0;
  saveJsonConfiguration();
  
  //load configuration file again an see if it's saved
  loadJsonConfiguration();
  printTestData();
}

void loop() {
  // not used in this example
}

void loadJsonConfiguration(void){
  SPIFFS.begin();
  File configFile = SPIFFS.open(filename, FILE_READ);

  const size_t capacity = 7*JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 330;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, configFile);
  
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
  
  JsonArray chrono_GIO = chrono["PIPPO"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.GIO[i] = chrono_GIO[i];
  }
  
  JsonArray chrono_VEN = chrono["VEN"];
  for (int i = 0; i < 24; i++)
  {
      config.chrono.calendar.VEN[i] = chrono_VEN[i];
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
}

void saveJsonConfiguration(void){
	SPIFFS.begin();
	SPIFFS.remove("/config.json");
  
  const size_t capacity = 7*JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 330;
	DynamicJsonDocument doc(capacity);
	File configFile = SPIFFS.open(filename, FILE_WRITE);
	
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
}

void printTestData(void){
  Serial.println("EVERYTHING FROM JSON IS: ");

  Serial.print("SSID: "); //TEST STRING
  Serial.println(config.network.SSID);
  Serial.print("IP: ");  //TEST STRING NUMBER
  Serial.println(config.network.ip);
  Serial.print("Eco setpoint: "); //REAL VALUE
  Serial.println(config.chrono.setpoints.eco);
  
  Serial.println("DOM temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.DOM[i]);
  }
  Serial.println("LUN temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.LUN[i]);
  }
  Serial.println("MAR temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.MAR[i]);
  }
  Serial.println("MER temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.MER[i]);
  }
  Serial.println("GIO temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.GIO[i]);
  }
  Serial.println("VEN temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.VEN[i]);
  }
  Serial.println("SAB temperatures: "); //INTVALUE
  for (int i=0; i < 4; i++){
    Serial.println(config.chrono.calendar.SAB[i]);
  }
  delay(1000);
}
