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
CONFIG settings;
ConfigParser json_parser(FILE_NAME);

//////////////////////////////////////
//PROJECT VARIABLES
//////////////////////////////////////
#include "Enumerators.h"
#include "Structures.h"
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>

// Task
Task SensorTask(1500, TASK_FOREVER, &SensorTaskCallback);
Task DisplayTask(1000, TASK_FOREVER, &DisplayTaskCallback);
Task ThermostatTask(3000, TASK_FOREVER, &ThermostatTaskCallback);
Task UserCommandsTask(0, TASK_FOREVER, &UserCommandsTaskCallback);
Task SerialDiagnosticTask(10000, TASK_FOREVER, &SerialDiagnosticCallback);

Scheduler runner;

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

float room_temperature = 0;
float room_humidity = 0;

DHT_Unified dht(IN_DHT, DHTTYPE);

//////////////////////////////////////
//THERMOSTAT
//////////////////////////////////////
#include "Thermostat.h"

Thermostat T(OUT_RELAY);

float temperature_setpoint = 0;

//////////////////////////////////////
//DISPLAY
//////////////////////////////////////
#include "Display.h"

Display disp = Display(TFT_LED, TFT_CS, TFT_CD);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// USER COMMANDS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UserCommands.h"
void buttonCallback();
UserCommands encoder(IN_ENC_A, IN_ENC_B, IN_ENC_BUTTON);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TELNET DEBUG
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RemoteDebug.h" //https://github.com/JoaoLopesF/RemoteDebug
#define TELNET_NAME "Thermostat"
RemoteDebug Debug;

/*
  debugV("* This is a message of debug level VERBOSE");
  debugD("* This is a message of debug level DEBUG");
  debugI("* This is a message of debug level INFO");
  debugW("* This is a message of debug level WARNING");
  debugE("* This is a message of debug level ERROR");
 */

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

SETPOINTS setpoints;

void setup()
{
  //read configuration file
  json_parser.loadConfiguration();
  settings = json_parser.config;

  //start communication
  InitConnection();
  delay(1000);

  //start all the modules
  disp.begin();
  encoder.begin();
  delay(200);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  //start tasks
  InitScheduler();

  //setup PINS
  pinMode(IN_ENC_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IN_ENC_BUTTON), ISR_callback, FALLING);
}

void loop()
{
  encoder.update();
  UserCommandsTaskCallback();
  runner.execute();

  // Remote debug over WiFi
  Debug.handle();
  yield();
}

void InitScheduler()
{
  runner.init();
  runner.addTask(SensorTask);
  runner.addTask(DisplayTask);
  runner.addTask(ThermostatTask);
  runner.addTask(UserCommandsTask);
  runner.addTask(SerialDiagnosticTask);

  SensorTask.enable();
  DisplayTask.enable();
  ThermostatTask.enable();
  UserCommandsTask.enable();
  SerialDiagnosticTask.enable();
}

void InitConnection()
{
  Conn.begin(settings.network.SSID,
             settings.network.password,
             settings.network.ip,
             settings.network.dns,
             settings.network.subnet,
             settings.network.gateway);

  while (Conn.connectionStatus() != WL_CONNECTED)
  {
    delay(500);
  }
  Conn.NTPBegin(123);
  delay(500);

  Conn.NTPSetServerName(settings.network.NTPServerName);
  Conn.NTPUpdateSystemTime();
  Debug.begin(TELNET_NAME);
  delay(500);
}

void SensorTaskCallback()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature))
  {
    debugE("Error reading temperature!");
  }
  else
  {
    room_temperature = event.temperature;
    debugV("Temperature: %s", room_temperature);
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    debugE("Error reading humidity!");
  }
  else
  {
    room_humidity = event.relative_humidity;
    debugV("Umidity: %s", room_humidity);
  }
}

void DisplayTaskCallback()
{
  switch (MACHINE_STATE)
  {
  case SPLASH_SCREEN: //SPLASH SCREEN SHOW
    debugV("[UPDATE] Display HomeScreen");
    disp.clearScreen();
    disp.showSplashScreen(VERSION);
    delay(2000);
    disp.clearScreen();
    MACHINE_STATE = MAIN_SCREEN;
    break;
  case MAIN_SCREEN:
    disp.showMainScreen(room_temperature, room_humidity, Conn.connectionStatus(), Conn.myIP().toString(), settings);
    break;
  case MENU_SCREEN:
    disp.showMenuScreen(MENU_INDEX);
    break;
  case BRIGHTNESS_SCREEN:
    disp.showBrightness(BRIGHT);
    disp.setBacklight(BRIGHT);
    break;
  case CHRONO_SCREEN:
    disp.showProgrammation(PROGRAMMING_CHRONO_NAV_INDEX, PROGRAMMING_CHRONO_DAY_INDEX, PROGRAMMING_CHRONO_TIME_INDEX, PROGRAMMING_CHRONO_TEMP_INDEX, settings);
    break;
  case SETPOINTS_SCREEN:
    double t_setpoint;
    switch (PROGRAMMING_SETPOINT_PROGRAM_INDEX)
    {
    case ECO:
      t_setpoint = settings.chrono.setpoints.eco;
      break;
    case NORMAL:
      t_setpoint = settings.chrono.setpoints.normal;
      break;
    case COMFORT:
      t_setpoint = settings.chrono.setpoints.comfort;
      break;
    case COMFORT_PLUS:
      t_setpoint = settings.chrono.setpoints.comfort_p;
      break;
    }
    disp.showTempSetpointsSettings(PROGRAMMING_SETPOINT_NAV_INDEX, PROGRAMMING_SETPOINT_PROGRAM_INDEX, t_setpoint, settings);
    break;
  }
}

void ThermostatTaskCallback()
{
  temperature_setpoint = getSetpoint();
  T.update(room_temperature, temperature_setpoint);
}

float getSetpoint()
{
  int day_of_week = weekday();

  int hour_of_day = hour();
  int setpoint_index = 0;
  switch (day_of_week)
  {
  case DOMENICA: //SUNDAY
    setpoint_index = settings.chrono.calendar.DOM[hour_of_day];
    break;
  case LUNEDI: //MONDAY
    setpoint_index = settings.chrono.calendar.LUN[hour_of_day];
    break;
  case MARTEDI: //TUESDAY
    setpoint_index = settings.chrono.calendar.MAR[hour_of_day];
    break;
  case MERCOLEDI: //WEDNESDAY
    setpoint_index = settings.chrono.calendar.MER[hour_of_day];
    break;
  case GIOVEDI: //THURSDAY
    setpoint_index = settings.chrono.calendar.GIO[hour_of_day];
    break;
  case VENERDI: //FRIDAY
    setpoint_index = settings.chrono.calendar.VEN[hour_of_day];
    break;
  case SABATO: //SATURDAY
    setpoint_index = settings.chrono.calendar.SAB[hour_of_day];
    break;
  }
  return indexToTemperature(setpoint_index);
}

float indexToTemperature(int index)
{
  switch (index)
  {
  case OFF:
    return TEMPERATURE_OFF;
    break;
  case ECO:
    return settings.chrono.setpoints.eco;
    break;
  case NORMAL:
    return settings.chrono.setpoints.normal;
    break;
  case COMFORT:
    return settings.chrono.setpoints.comfort;
    break;
  case COMFORT_PLUS:
    return settings.chrono.setpoints.comfort_p;
    break;
  }
}

void UserCommandsTaskCallback()
{

  if (encoder.turnedLeft())
  {
    debugV("LEFT COMMAND");
    switch (MACHINE_STATE)
    {
    case MENU_SCREEN:
      if (MENU_INDEX > 0)
        MENU_INDEX--;
      break;
    case BRIGHTNESS_SCREEN:
      if (BRIGHT > 10)
        BRIGHT = BRIGHT - 10;
      break;
    case CHRONO_SCREEN:
      switch (PROGRAMMING_CHRONO_NAV_INDEX)
      {
      case CHOOSE_DAY:
        PROGRAMMING_CHRONO_DAY_INDEX--;
        if (PROGRAMMING_CHRONO_DAY_INDEX < DOMENICA)
          PROGRAMMING_CHRONO_DAY_INDEX = SABATO;
        break;
      case CHOOSE_TIME:
        PROGRAMMING_CHRONO_TIME_INDEX--;
        if (PROGRAMMING_CHRONO_TIME_INDEX < 0)
          PROGRAMMING_CHRONO_TIME_INDEX = 24;
        break;
      case CHOOSE_TEMPERATURE:
        PROGRAMMING_CHRONO_TEMP_INDEX--;
        if (PROGRAMMING_CHRONO_TEMP_INDEX < OFF)
          PROGRAMMING_CHRONO_TEMP_INDEX = COMFORT_PLUS;
        break;
      }
      break;
    case SETPOINTS_SCREEN:
      switch (PROGRAMMING_SETPOINT_NAV_INDEX)
      {
      case CHOOSE_PROGRAM:
        PROGRAMMING_SETPOINT_PROGRAM_INDEX--;
        if (PROGRAMMING_SETPOINT_PROGRAM_INDEX < OFF)
          PROGRAMMING_SETPOINT_PROGRAM_INDEX = COMFORT_PLUS;
        break;
      case CHOOSE_SETPOINT:
        switch (PROGRAMMING_SETPOINT_PROGRAM_INDEX)
        {
        case ECO:
          settings.chrono.setpoints.eco -= PROGRAMMING_SETPOINT_INCREMENT;
          break;
        case NORMAL:
          settings.chrono.setpoints.normal -= PROGRAMMING_SETPOINT_INCREMENT;
          break;
        case COMFORT:
          settings.chrono.setpoints.comfort -= PROGRAMMING_SETPOINT_INCREMENT;
          break;
        case COMFORT_PLUS:
          settings.chrono.setpoints.comfort_p -= PROGRAMMING_SETPOINT_INCREMENT;
          break;
        }
        break;
      }
      break;
    }
  }

  if (encoder.turnedRight())
  {
    debugV("RIGHT COMMAND");
    switch (MACHINE_STATE)
    {
    case MENU_SCREEN:
      if (MENU_INDEX < BACK)
        MENU_INDEX++;
      break;
    case BRIGHTNESS_SCREEN:
      if (BRIGHT < 100)
        BRIGHT = BRIGHT + 10;
      break;
    case CHRONO_SCREEN:
      switch (PROGRAMMING_CHRONO_NAV_INDEX)
      {
      case CHOOSE_DAY:
        PROGRAMMING_CHRONO_DAY_INDEX++;
        if (PROGRAMMING_CHRONO_DAY_INDEX > SABATO)
          PROGRAMMING_CHRONO_DAY_INDEX = 0;
        break;
      case CHOOSE_TIME:
        PROGRAMMING_CHRONO_TIME_INDEX++;
        if (PROGRAMMING_CHRONO_TIME_INDEX > 24)
          PROGRAMMING_CHRONO_TIME_INDEX = 0;
        break;
      case CHOOSE_TEMPERATURE:
        PROGRAMMING_CHRONO_TEMP_INDEX++;
        if (PROGRAMMING_CHRONO_TEMP_INDEX > COMFORT_PLUS)
          PROGRAMMING_CHRONO_TEMP_INDEX = 0;
        break;
      }
      break;
    case SETPOINTS_SCREEN:
      switch (PROGRAMMING_SETPOINT_NAV_INDEX)
      {
      case CHOOSE_PROGRAM:
        PROGRAMMING_SETPOINT_PROGRAM_INDEX++;
        if (PROGRAMMING_SETPOINT_PROGRAM_INDEX > COMFORT_PLUS)
          PROGRAMMING_SETPOINT_PROGRAM_INDEX = OFF;
        break;
      case CHOOSE_SETPOINT:
        //increase actual setpoint
        switch (PROGRAMMING_SETPOINT_PROGRAM_INDEX)
        {
        case ECO:
          settings.chrono.setpoints.eco += PROGRAMMING_SETPOINT_INCREMENT;
          break;
        case NORMAL:
          settings.chrono.setpoints.normal += PROGRAMMING_SETPOINT_INCREMENT;
          break;
        case COMFORT:
          settings.chrono.setpoints.comfort += PROGRAMMING_SETPOINT_INCREMENT;
          break;
        case COMFORT_PLUS:
          settings.chrono.setpoints.comfort_p += PROGRAMMING_SETPOINT_INCREMENT;
          break;
        }
        break;
      }
      break;
    }
  }

  /*
   ****************
   * NAVIGATION
   ****************
   */
  if (encoder.buttonWasPressed())
  {
    debugV("BUTTON PRESSED");
    switch (MACHINE_STATE)
    {
    case MAIN_SCREEN:
      disp.clearScreen();
      MACHINE_STATE = MENU_SCREEN;
      break;
    case MENU_SCREEN:
      disp.clearScreen();
      switch (MENU_INDEX)
      {
      case PROGRAMMING:
        disp.clearScreen();
        MACHINE_STATE = CHRONO_SCREEN;
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
    case CHRONO_SCREEN:
      switch (PROGRAMMING_CHRONO_NAV_INDEX)
      {
      case CHOOSE_DAY:
        if (PROGRAMMING_CHRONO_DAY_INDEX == NO_DAY)
        {
          disp.clearScreen();
          MACHINE_STATE = MENU_SCREEN;
        }
        else
        {
          PROGRAMMING_CHRONO_NAV_INDEX++;
        }
        break;
      case CHOOSE_TIME:
        if (PROGRAMMING_CHRONO_TIME_INDEX == 24)
        {
          PROGRAMMING_CHRONO_NAV_INDEX--;
        }
        else
        {
          PROGRAMMING_CHRONO_NAV_INDEX++;
        }
        break;
      case CHOOSE_TEMPERATURE:
        PROGRAMMING_CHRONO_NAV_INDEX--;
        //SELECTION CONFIRMED
        //SAVE SETTINGS
        switch (PROGRAMMING_CHRONO_DAY_INDEX)
        {
        case DOMENICA:
          settings.chrono.calendar.DOM[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        case LUNEDI:
          settings.chrono.calendar.LUN[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        case MARTEDI:
          settings.chrono.calendar.MAR[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        case MERCOLEDI:
          settings.chrono.calendar.MER[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        case GIOVEDI:
          settings.chrono.calendar.GIO[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        case VENERDI:
          settings.chrono.calendar.VEN[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        case SABATO:
          settings.chrono.calendar.SAB[PROGRAMMING_CHRONO_TIME_INDEX] = PROGRAMMING_CHRONO_TEMP_INDEX;
          break;
        }
        json_parser.config = settings;   //update structure
        json_parser.saveConfiguration(); //save new settings
        break;
      }
      break;
    case SETPOINTS_SCREEN:
      switch (PROGRAMMING_SETPOINT_NAV_INDEX)
      {
      case CHOOSE_PROGRAM:
        if (PROGRAMMING_SETPOINT_NAV_INDEX == OFF)
        {
          json_parser.config = settings;   //update structure
          json_parser.saveConfiguration(); //save new settings
        }
        else
          PROGRAMMING_SETPOINT_NAV_INDEX++;
        break;
      case CHOOSE_SETPOINT:
        PROGRAMMING_SETPOINT_NAV_INDEX--;
        break;
      }
      break;
    }
  }
}

void SerialDiagnosticCallback()
{
  debugI("[UPDATE] Reading sensors");
  debugI("Room temperature: %f", room_temperature);
  debugI("Room humidity: %f", room_humidity);
  debugI("[UPDATE] Update Thermostat");
  debugI("Temperature setpoint: %f ", temperature_setpoint);
  debugI("Thermostat status: %s", T.get_heater_state() ? "ON" : "OFF");
  debugI("Week of the day: %d", weekday());
  debugI("Check configuration match:");
  debugI("Parser: %d", json_parser.config.chrono.calendar.GIO[0]);
  debugI("Settings: %d", settings.chrono.calendar.GIO[0]);
  yield();
}

int button_debounce = 50;
int ms_button = 0;

void ISR_callback()
{
  if (millis() - ms_button > button_debounce)
    encoder.buttonInterruptHandler();
  ms_button = millis();
}
