// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

#include "FS.h"
#include <ArduinoJson.h>

const char* network_net_name;
const char* network_net_pass;
const char* network_ip;
const char* network_sub;
const char* network_dns;
const char* network_gateway;
const char* network_ntp_server;
int setpoints_eco;
int setpoints_normal;
int setpoints_comfort;
int setpoints_comfort_plus;

void setup() {
  // Initialize Serial port
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
  network_net_name = network["net-name"]; // "\"TIM-90267905\""
  network_net_pass = network["net-pass"]; // "\"ufSzgF1bBGDlMWhhhG7OYrTC\""
  network_ip = network["ip"]; // "\"192.168.1.222\""
  network_sub = network["sub"]; // "\"255.255.255.0\""
  network_dns = network["dns"]; // "\"192.168.1.1\""
  network_gateway = network["gateway"]; // "\"192.168.1.1\""
  network_ntp_server = network["ntp-server"]; // "\"0.it.pool.ntp.org\""
  
  JsonObject setpoints = doc["setpoints"];
  setpoints_eco = setpoints["eco"]; // 18
  setpoints_normal = setpoints["normal"]; // 21
  setpoints_comfort = setpoints["comfort"]; // 23
  setpoints_comfort_plus = setpoints["comfort+"]; // 25
  
  JsonObject chrono = doc["chrono"];
}

void loop() {
  // not used in this example
  Serial.println("Setpoint comfort: ");
  Serial.println(setpoints_comfort);
  delay(1000);
}

// Visit https://arduinojson.org/v6/example/generator/ for more.
