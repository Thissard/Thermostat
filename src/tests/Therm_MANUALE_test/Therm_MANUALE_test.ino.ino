#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            2         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//THERMOSTAT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Thermostat.h"
#define OUT_RELAY 12

uint32_t delayMS;

//START AUTO
Thermostat T(OUT_RELAY,AUTOMATIC_MODE);

void setup() {
  Serial.begin(9600); 
  dht.begin();
  // put your setup code here, to run once:
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  Serial.println("Passaggio in manuale!");
  T.set_mode(MANUAL_MODE);
  
  for (int fadeValue = 0 ; fadeValue <= 70; fadeValue += 5){
      sensors_event_t event; 
      dht.temperature().getEvent(&event);
      float room_temp = event.temperature;
      float fake_temp = fadeValue*1.0;
      if (fadeValue >20 and fadeValue<60)
      T.turn_heater_ON();
      else
      T.turn_heater_OFF();
      
      if (isnan(event.temperature)) {
        Serial.println("Error reading temperature!");
      }
      else {
        Serial.print("SP Temperature: ");
        Serial.print(fake_temp);
        Serial.println(" *C");
        Serial.print("RoomTemperature: ");
        Serial.print(room_temp);
        Serial.println(" *C");
        T.update(room_temp,fake_temp);
        if (T.get_heater_state())
          Serial.println("in MANUALE Caldaia Accesa!");
        else
        Serial.println("in MANUALE Caldaia Spenta!");
      }
     
    delay(delayMS);
    
  }
  //passa ad auto
  Serial.println("Passaggio ad automatico!");
  T.set_mode(AUTOMATIC_MODE);

  for (int fadeValue = 70 ; fadeValue >= 0; fadeValue -= 5) {
  
      sensors_event_t event; 
      dht.temperature().getEvent(&event);
      float room_temp = event.temperature;
      float fake_temp = fadeValue*1.0;
      
      if (isnan(event.temperature)) {
        Serial.println("Error reading temperature!");
      }
      else {
        Serial.print("SP Temperature: ");
        Serial.print(fake_temp);
        Serial.println(" *C");
        Serial.print("RoomTemperature: ");
        Serial.print(room_temp);
        Serial.println(" *C");
        T.update(room_temp,fake_temp);
        if (T.get_heater_state())
          Serial.println("Caldaia Accesa!");
        else
        Serial.println("Caldaia Spenta!");
      }

    delay(delayMS);
    
  }
}
