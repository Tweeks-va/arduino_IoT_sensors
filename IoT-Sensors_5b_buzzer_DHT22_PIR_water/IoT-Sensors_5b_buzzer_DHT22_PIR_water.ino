//DHT22 Defs and Variables
// Based on example sketch for various DHT humidity/temperature sensors
// by ladyada, public domain
#include "DHT.h"
#define DHTPIN 4        // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22 (aka AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
const int buzzerPin = 3;           // Sound Buzzer on pin#

//PIR Motion Sensor variables:
const int calibrationTime = 10; //the time we give the sensor to calibrate (20­30sec)
const int pirPin = 2;     //the digital pin connected to the PIR sensor's output
const int ledPin = 13;    // the built in LED pin

//Water Sensor variables:
const int waterPin = A0;
int waterVal = 0;


void setup() {
  //PIR Motion Sensor Setup
  Serial.begin(9600);
  pinMode(pirPin, INPUT);       // Set up the various I/O pins
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  //give the sensor some time to calibrate (monitor state on the serial port)
  Serial.print("MOTION: calibrating PIR motion sensor.");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println("");
    Serial.println("MOTION: PIR calibraton done");
    Serial.println("MOTION: SENSOR ACTIVE");
    delay(50);
  
  // DHT22 Setup
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

  /////////// PIR Motion Sensor Code Block
  ////////////////////////////////////////////////////
    if(digitalRead(pirPin) == HIGH){        // If the PIR is HIGH
       digitalWrite(ledPin, HIGH);        // then turn on the LED pin and
       Serial.println("MOTION: HIGH (motion)");   // print "HIGH (motion)" on serial port
  }
  else {
       digitalWrite(ledPin, LOW);         // else, make the LED pin low and
       Serial.println("MOTION: LOW (no motion)"); // print "LOW (no motion)" on serial port
  }

  ////////// Water Sensor Code Block
  ////////////////////////////////////////////////////
  waterVal = analogRead(waterPin);
  Serial.print("WATER: ");Serial.println(waterVal);
  


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

