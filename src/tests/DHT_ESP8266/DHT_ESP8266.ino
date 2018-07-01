#include "DHTesp.h"

#define DHT_DATA D6

DHTesp dht;

void setup()
{
  Serial.begin(9600);
  Serial.println();
   
  dht.setup(DHT_DATA, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
  Serial.println();
  Serial.print("Umidità: ");
  Serial.println(humidity, 1);
  Serial.print("Temperatura: ");
  Serial.println(temperature, 1);
  Serial.print("Percepita: ");
  Serial.println(dht.computeHeatIndex(temperature, humidity, false), 1);
  
}

