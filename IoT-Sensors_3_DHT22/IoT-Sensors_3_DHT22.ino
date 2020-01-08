// Based on example sketch for various DHT humidity/temperature sensors
// by ladyada, public domain
#include "DHT.h"
#define DHTPIN 4        // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22 (aka AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT­22 test!");
  dht.begin();
}

void loop() {
    delay(2000);
// Wait a few seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its very slow)
  float h = dht.readHumidity();
  float t = dht.readTemperature();        // Read temperature as °C
  float f = dht.readTemperature(true);    // Read temperature as °F 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float hif = dht.computeHeatIndex(f, h);         // Compute heat index °F
  float hic = dht.computeHeatIndex(t, h, false);  // Compute heat index °C
  Serial.print("Humidity: "); Serial.print(h); Serial.print(" %\t");
  Serial.print("Temperature: "); Serial.print(t)  ; Serial.print(" °C (");   
  Serial.print(f); Serial.print(" °F)\t\t");
  Serial.print("Heat index: "); Serial.print(hic); Serial.print(" °C ("); 
  Serial.print(hif); Serial.println(" °F)");
}
