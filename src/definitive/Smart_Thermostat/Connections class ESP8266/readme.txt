How to use it:

//////////////////////////////////////
//WIFI + NTP + MQTT
//////////////////////////////////////
#include "Connections.h"

Connections Conn(conf_SSID,conf_password,conf_ip,conf_dns,conf_subnet,conf_gateway);





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