// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

#include "FS.h"
#include <ArduinoJson.h>

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println("\nVery basic Spiffs example, writing 10 lines to SPIFFS filesystem, and then read them back");
  SPIFFS.begin();
  // Next lines have to be done ONLY ONCE!!!!!When SPIFFS is formatted ONCE you can comment these lines out!!
  Serial.println("Please wait 30 secs for SPIFFS to be formatted");
  //SPIFFS.format();
  Serial.println("Spiffs formatted");
  
  File f = SPIFFS.open("/chrono.json", "w");

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);

  if (error)
  Serial.println(F("Failed to read file, using default configuration"));

  // Get the root object in the document
  JsonObject root = doc.as<JsonObject>();
  
}

void loop() {
  // not used in this example
}

// Visit https://arduinojson.org/v6/example/generator/ for more.
