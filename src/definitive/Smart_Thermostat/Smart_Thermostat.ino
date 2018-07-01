//////////////////////////////////////
//PROJECT CONFIGURATION
//////////////////////////////////////
#include "Configurations.h"
#define VERSION "v0.0.1"

//////////////////////////////////////
//WIFI + NTP + MQTT
//////////////////////////////////////
#include "Connections.h"

Connections Conn(conf_SSID,conf_password,conf_ip,conf_dns,conf_subnet,conf_gateway);

//////////////////////////////////////
// TEMPERATURE SENSOR
//////////////////////////////////////
#include "DHTesp.h"

float room_temperature=0;
float room_humidity=0;

DHTesp dht;

//////////////////////////////////////
//THERMOSTAT
//////////////////////////////////////
#include "Thermostat.h"

Thermostat T(OUT_RELAY);

float temperature_setpoint;

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
//CODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  Serial.begin(9600);
  InitConnection();
  delay(1000);
  
  dht.setup(IN_DHT, DHTesp::DHT22);
  disp.begin();
  
}

void loop() {
  Serial.println("[UPDATE] Reading sensors");
  readSensor();
  T.update(room_temperature, temperature_setpoint);         //TODO EVERY SECOND
  
  delay(1000);

  Serial.println("[UPDATE] Display HomeScreen");
  disp.clearScreen();
  disp.showSplashScreen(VERSION);
  delay(3000);
  disp.clearScreen();
  disp.showMainScreen(24.4 , 33.6, Conn.connectionStatus(), Conn.myIP().toString(), colors);
  delay(10000);
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

void readSensor(){
  delay(dht.getMinimumSamplingPeriod());
  room_humidity = dht.getHumidity();
  int temp = room_humidity *10;
  room_humidity = temp/10;
  
  room_temperature = dht.getTemperature();
  temp = room_temperature *10;
  room_temperature = temp/10;
  
  Serial.print("[UPDATE] Room temperature: ");
  Serial.println(room_temperature);
  Serial.print("[UPDATE] Room humidity: ");
  Serial.println(room_humidity);
}








/*
unsigned long TFT_Draw_left_circle(void){
  unsigned long start = micros();
    tft.fillCircle(80,120,78, ILI9341_GREEN);
    tft.fillCircle(80,120,68, ILI9341_BLACK);
  return micros()- start;
}

unsigned long TFT_Draw_right_circle(void){
  unsigned long start = micros();
    tft.fillCircle(240,120,58, ILI9341_WHITE);
    tft.fillCircle(240,120,48, ILI9341_BLACK);
  return micros()- start;
}





unsigned long TFT_writeTemp_text(void){
  unsigned long start = micros();
    tft.setFont(&Gameplay20pt7b);
    tft.setCursor(5,120);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("24");
    tft.setCursor(125,120);
    tft.setTextSize(1);
    tft.print("2");
    tft.setFont();
  return micros()- start;
}


*/
