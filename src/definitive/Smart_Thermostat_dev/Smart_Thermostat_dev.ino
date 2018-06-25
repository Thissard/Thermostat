#include "Configurations.h"

//WIFI + NTP + MQTT
#include "Connections.h"

//USA IL TIPO DI CONNESSIONE CHE VUOI
//Connections Conn(conf_SSID,conf_password,conf_mqtt_broker);
Connections Conn(conf_SSID,conf_password,conf_ip,conf_dns,conf_subnet,conf_gateway,conf_mqtt_broker);

void setup() {
  Serial.begin(9600);
  Serial.println("Wifi Test!");
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

time_t prevDisplay = 0; // when the digital clock was displayed

void loop() {
  // put your main code here, to run repeatedly:
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      // digital clock display of the time
      Serial.print(hour());
      Serial.print(":");
      if (minute() < 10)
        Serial.print('0');
      Serial.print(minute());
      Serial.print(":");
      if (second() < 10)
        Serial.print('0');
      Serial.print(second());
      Serial.print(" ");
      Serial.print(day());
      Serial.print("/");
      Serial.print(month());
      Serial.print("/");
      Serial.print(year());
      Serial.println();
  }
}
