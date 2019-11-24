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
#define VERSION "v0.0.1"

#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>

// Task
Task SensorTask(1500, TASK_FOREVER,&SensorTaskCallback);
Task DisplayTask(1000, TASK_FOREVER,&DisplayTaskCallback);
Task ThermostatTask(3000, TASK_FOREVER,&ThermostatTaskCallback);
Task UserCommandsTask(0,TASK_FOREVER,&UserCommandsTaskCallback);
Task SerialDiagnosticTask(10000, TASK_FOREVER,&SerialDiagnosticCallback);

Scheduler runner;

//////////////////////////////////////
//WIFI + NTP + MQTT
//////////////////////////////////////
#include "Connections.h"

Connections Conn(conf_SSID,conf_password,conf_ip,conf_dns,conf_subnet,conf_gateway);

//////////////////////////////////////
// TEMPERATURE SENSOR
//////////////////////////////////////
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

float room_temperature=0;
float room_humidity=0;

DHT_Unified dht(IN_DHT, DHTTYPE);

//////////////////////////////////////
//THERMOSTAT
//////////////////////////////////////
#include "Thermostat.h"

Thermostat T(OUT_RELAY);

float temperature_setpoint=0;

//////////////////////////////////////
//DISPLAY
//////////////////////////////////////
#include "Display.h"

Display disp = Display(TFT_LED, TFT_CS, TFT_CD);

uint16_t colors[24] = {
    ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, // 00 01 02 03
    ILI9341_BLUE, ILI9341_BLUE, ILI9341_GREEN, ILI9341_GREEN, // 04 05 06 07
    ILI9341_GREEN, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, // 08 09 10 11
    ILI9341_BLUE, 0x2924, 0x2924, ILI9341_BLUE, // 12 13 14 15
    ILI9341_ORANGE, ILI9341_ORANGE, ILI9341_ORANGE, ILI9341_ORANGE, // 16 17 18 19
    ILI9341_RED, ILI9341_RED, ILI9341_GREEN, ILI9341_GREEN // 20 21 22 23
    };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// USER COMMANDS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UserCommands.h"
void buttonCallback();
UserCommands encoder(IN_ENC_A, IN_ENC_B, IN_ENC_BUTTON);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long old_ms_display = 0;
long old_ms_encoder = 0;
long old_ms_dht = 0;
long old_ms_thermostat = 0;
long old_ms_full_cycle = 0;

long ms_display = 0;
long ms_encoder = 0;
long ms_dht = 0;
long ms_thermostat = 0;
long ms_full_cycle = 0;

void setup(){
  Serial.begin(9600);
  InitConnection();
  delay(1000);

  disp.begin();
  encoder.begin();
  delay(200);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  
  InitScheduler();

  pinMode(IN_ENC_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IN_ENC_BUTTON), ISR_callback , FALLING );
}

void loop() {
  encoder.update();
  //UserCommandsTaskCallback();
  runner.execute();
}

void InitScheduler(){
  runner.init();
  Serial.println("Initialized scheduler");
  runner.addTask(SensorTask);
  Serial.println("SensorTask scheduled");
  runner.addTask(DisplayTask);
  Serial.println("DisplayTask scheduled");
  runner.addTask(ThermostatTask);
  Serial.println("ThermostatTask scheduled");
  runner.addTask(UserCommandsTask);
  Serial.println("UserCommandsTask scheduled");
  runner.addTask(SerialDiagnosticTask);
  Serial.println("SerialDiagnosticTask scheduled");
  
  SensorTask.enable();
  Serial.println("SensorTask enabled");
  DisplayTask.enable();
  Serial.println("DisplayTask enabled");
  ThermostatTask.enable();
  Serial.println("ThermostatTask enabled");
  UserCommandsTask.enable();
  Serial.println("UserCommandsTask enabled");
  SerialDiagnosticTask.enable();
  Serial.println("SerialDiagnosticTask enabled");
  }

void InitConnection(){
  Serial.println("Smart Thermostat Initialization");
  Serial.print("SSID: ");
  Serial.println(conf_SSID);
  Serial.print("Connecting.");
  Conn.begin();
  while (Conn.connectionStatus() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("IP: ");
  Serial.println(Conn.myIP());
  Serial.println("NTP Connection start");
  Conn.NTPBegin(123);
  delay(500);
  Serial.println("NTP Connection established");
  Serial.println("Setting NTP server name");
  Conn.NTPSetServerName(NTPServerName);
  if (Conn.NTPUpdateSystemTime()==0)
   Serial.println("Cannot get server response");
  Serial.println("");
}

void SensorTaskCallback(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    room_temperature = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    room_humidity = event.relative_humidity;
  }

}

void DisplayTaskCallback(){
  switch (MACHINE_STATE){
    case SPLASH_SCREEN: //SPLASH SCREEN SHOW
      Serial.println("[UPDATE] Display HomeScreen");
      disp.clearScreen();
      disp.showSplashScreen(VERSION);
      delay(2000);
      disp.clearScreen();
      MACHINE_STATE=MAIN_SCREEN;
      break;
    case MAIN_SCREEN:
      disp.showMainScreen(room_temperature , room_humidity, Conn.connectionStatus(), Conn.myIP().toString(), colors);
      break;
    case MENU_SCREEN:
      disp.showMenuScreen(INDEX);
      break;
    case BRIGHTNESS_SCREEN:
      disp.showBrightness(BRIGHT);
      disp.setBacklight(BRIGHT);
      break;
    case CHRONO_SCREEN:
      break;
  }
}

void ThermostatTaskCallback(){

  T.update(room_temperature, temperature_setpoint);         //TODO EVERY SECOND

  if (T.get_heater_state())
    temperature_setpoint = room_temperature - 5;
  else
    temperature_setpoint = room_temperature + 5;

}

void UserCommandsTaskCallback(){
  
  if (encoder.turnedLeft()){
    Serial.println("LEFT COMMAND");
    switch (MACHINE_STATE){
      case MENU_SCREEN:
        if (INDEX > 0)INDEX--;
       break;
      case BRIGHTNESS_SCREEN:
        if (BRIGHT > 10) BRIGHT = BRIGHT - 10;
       break;
    }
  }
  
  if (encoder.turnedRight()){
    Serial.println("RIGHT COMMAND");
    switch (MACHINE_STATE){
      case MENU_SCREEN:
        if (INDEX < BACK) INDEX++;
       break;
      case BRIGHTNESS_SCREEN:
        if (BRIGHT < 100) BRIGHT = BRIGHT + 10;
       break;
    }
  }
  /*
   ****************
   * NAVIGATION
   ****************
   */
  if (encoder.buttonWasPressed()){
    Serial.println("BUTTON PRESSED");
    switch (MACHINE_STATE){
      case MAIN_SCREEN:
        disp.clearScreen();
        MACHINE_STATE = MENU_SCREEN;
        break;
      case MENU_SCREEN:
        disp.clearScreen();
        switch (INDEX){
          case PROGRAMS:
          break;
          case BRIGHTNESS:
            disp.clearScreen();
            MACHINE_STATE = BRIGHTNESS_SCREEN;
          break;
          case BACK:
            disp.clearScreen();
            MACHINE_STATE = MAIN_SCREEN;
          break;
        }
        break;
      case BRIGHTNESS_SCREEN:
        disp.clearScreen();
        MACHINE_STATE = MENU_SCREEN;
        break;  
    }
  }
}

void SerialDiagnosticCallback(){
  
  Serial.println("[UPDATE] Reading sensors");
  Serial.print("Room temperature: ");
  Serial.println(room_temperature);
  Serial.print("Room humidity: ");
  Serial.println(room_humidity);
  Serial.println("[UPDATE] Update Thermostat");
  Serial.print("Temperature setpoint: ");
  Serial.println(temperature_setpoint);
  Serial.print(" Thermostat status: ");
  Serial.println(T.get_heater_state() ? "ON" : "OFF");
  yield();
}

int button_debounce= 50;
int ms_button = 0;

void ISR_callback(){
  if (millis() - ms_button > button_debounce)
    encoder.buttonInterruptHandler();
  ms_button = millis();
}
