// Based on example sketch for various DHT humidity/temperature sensors
// by ladyada, public domain
#include "DHT.h"
#define DHTPIN 4        // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22 (aka AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
const int buzzerPin = 3;           // Sound Buzzer on pin#

void setup() {
  // DHT22 Setup
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);  
  digitalWrite(buzzerPin, HIGH); 
  Serial.println("HUMIDITY: DHT22 ready...");
  dht.begin();

Serial.println("=========================");
}

// loop()
//////////////////////////////////////////////////////
void loop() {
    delay(2000);

  ////////////// DHT22 Humidity / Temp Block
  // Wait a few seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its very slow)
  float h = dht.readHumidity();
  float t = dht.readTemperature();        // Read temperature as °C
  float f = dht.readTemperature(true);    // Read temperature as °F 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    buzzer("fail");
    return;
  }
  float hif = dht.computeHeatIndex(f, h);         // Compute heat index °F
  float hic = dht.computeHeatIndex(t, h, false);  // Compute heat index °C
  buzzer("chirp");
  Serial.print("HUMIDITY: "); Serial.print(h); Serial.print(" %\t"); 
  Serial.print("Temperature: "); Serial.print(f); Serial.println(" °F");

Serial.println("=========================");

  
}

//// buzzer("success" || "fail" || "chirp" || "alarm" ) ////
////////////////////////////////////////////////////////////
void buzzer(char mode[]){
  if(mode == "success"){
    tone(buzzerPin, 2000); delay(50); noTone(buzzerPin); delay(50); 
    tone(buzzerPin, 2000); delay(50); noTone(buzzerPin);    
  }
  if(mode == "fail"){
    tone(buzzerPin, 400, 400); delay(400); 
    tone(buzzerPin, 100, 1500); delay(1500); 
    noTone(buzzerPin);  
  }
  if(mode == "chirp"){
  tone(buzzerPin, 3000); delay(50); noTone(buzzerPin);
  }
  if(mode == "alarm"){
    for (int x=0 ; x<1 ; x++){
     tone(buzzerPin, 1000, 1000); delay(1000);
     tone(buzzerPin, 800, 1000); delay(1000);
     noTone(buzzerPin);  
    }
  }
}

