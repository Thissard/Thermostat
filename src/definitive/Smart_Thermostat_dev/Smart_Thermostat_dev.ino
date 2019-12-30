////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SMART THERMOSTAT PROJECT
//AUTHOR: MAURIZIO DE BENETTI
//LIBRARIES AND VERSIONS:
//ESP - Arduino core for ESP8266 WiFi chip - V2.4.2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
//PROJECT CONFIGURATION
//////////////////////////////////////
#include "Configurations.h"
#include "ConfigParser.h"
#define VERSION "v0.0.1"

ConfigParser settings(FILE_NAME);

//////////////////////////////////////
//PROJECT VARIABLES
//////////////////////////////////////
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>

// Task

//////////////////////////////////////
//WIFI + NTP + MQTT
//////////////////////////////////////
#include "Connections.h"
Connections Conn;

//////////////////////////////////////
// TEMPERATURE SENSOR
//////////////////////////////////////
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//////////////////////////////////////
//THERMOSTAT
//////////////////////////////////////
#include "Thermostat.h"
//////////////////////////////////////

//////////////////////////////////////
//DISPLAY
//////////////////////////////////////
#include "Display.h"
//////////////////////////////////////

//////////////////////////////////////
// USER COMMANDS
//////////////////////////////////////
#include "UserCommands.h"
//////////////////////////////////////

//////////////////////////////////////
// TELNET DEBUG
//////////////////////////////////////
#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
#define TELNET_NAME "Thermostat"
RemoteDebug Debug;

/*
  debugV("* This is a message of debug level VERBOSE");
  debugD("* This is a message of debug level DEBUG");
  debugI("* This is a message of debug level INFO");
  debugW("* This is a message of debug level WARNING");
  debugE("* This is a message of debug level ERROR");
 */
//////////////////////////////////////

//////////////////////////////////////
// CODE
//////////////////////////////////////

void setup(){
  delay(1000);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Sync,Sync,Sync,Sync,Sync");
  delay(500);
  Serial.println();
  settings.loadConfiguration();

  Conn.begin(settings.config.network.SSID,
            settings.config.network.password,
            settings.config.network.ip,
            settings.config.network.dns,
            settings.config.network.subnet,
            settings.config.network.gateway);
  
  
  Debug.begin(TELNET_NAME);
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
  Debug.showColors(true); // Colors
}

const long interval = 1000;           // interval at which to blink (milliseconds)
unsigned long previousMillis = 0;        // will store last time LED was updated

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
    Serial.println(settings.config.network.ip);
    debugI("%s",settings.config.network.ip.c_str());
    
    }
  
// Remote debug over WiFi
  Debug.handle();
  yield();
}
