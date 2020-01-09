// IoT-Sensors_6b_complete
////////////////////////////////////////////////
// By tweeks-homework@theweeks.org, v2020-01-08
// https://github.com/LetsCodeBlacksburg/arduino_IoT_sensors/ 
// This "feature complete" version of this lab's code integrates
// all components and sensors in this lab (buzzer, humidity/temp, 
// PIR/motion, and water sensor) and integrates them along with
// an ESP01 WiFi module to put your sensor node on line (web by
// default, but SMTP/SMS can be implimented). Please send any
// pull requests to the above git repo.


// Wireless Libraries & Variables
///////////////////////////////////////
// WiFiESP by bportaluri @ https://github.com/bportaluri/WiFiEsp
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
#include "WiFiEsp.h"
// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // Using these pis for Software Serial RX, TX
#endif

// ESP8266 Variables
char ssid[] = "";        // the network SSID (name) you're connecting to
char pass[] = "";       // the network password (blank if none/open)
int status = WL_IDLE_STATUS;            // the Wifi radio's status
int reqCount = 0;                       // number of requests received
WiFiEspServer server(80);
const int buzzerPin = 3;          // Sound Buzzer



// Sensor Variables
////////////////////////////////////////
//PIR Motion Sensor variables:
const int calibrationTime = 8; //the time we give the sensor to calibrate (20­30sec)
const int pirPin = 2;     //the digital pin connected to the PIR sensor's output
const int ledPin = 13;    // the built in LED pin
// for Web
char sensorPIRHeader[] = "Motion State = ";
char sensorPIRData[] = " ";


// Water Sensor variables:
const int waterPin = A0;
int waterVal = 0;
// for Web
char sensorWaterHeader[] = "Water Level (0-1023) = ";
int sensorWaterData = 0;


// See DHT­22 recipe 
//DHT22 Defs and Variables
// by ladyada, public domain
#include "DHT.h"
#define DHTPIN 4        // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22 (aka AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
// for Web
char sensorDHT22THeader[] = "Current Temperature(°F) = ";
float sensorDHT22TData = 0;
// See DHT­22 recipe
char sensorDHT22HHeader[] = "Current Humidity(%) = ";
float sensorDHT22HData = 0;

///////// Setup 
////////////////////////////////
void setup()
{
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
  
  ////// Setup WiFi
  //Serial.begin(9600);       // initialize serial for debugging

  ////// Serial-Hack to get ESP Talking at 9600 (default is 115.2k)
  // If this 115k-->9600 hack doesn't work, you may need to set your
  // Arduino UNOs UART speed manually via a terminal and serial link as per
  // https://create.arduino.cc/projecthub/Imalwaysontheinternet/esp8266-setup-and-first-wifi-connection-76fc3c
  Serial1.begin(115200);      // Can leave at 115.2 if on an Arduino mega
  delay(250);
  Serial1.println("AT+UART_DEF=9600,8,1,0,0");  // Ititializes ESP to talk at 9600
  delay(250);
  Serial1.begin(9600);      // sets Arduino Software Serial1 back to 9600
  delay(250);
  WiFi.init(&Serial1);      // initialize ESP module
 

  if (WiFi.status() == WL_NO_SHIELD) {    // check for ESP­01
    Serial.println("WiFi shield not present");
    buzzer("fail");         // hmmm.. this would be useful if it worked.
    while (true);             // don't continue
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);    // Connect to WPA/WPA2 network
  }
  buzzer("success");              // Connected
  Serial.println("You're connected to the network");
  printWifiStatus();
  server.begin();    // start the web server on port 80
}


//// loop()
/////////////////////////////////////
void loop()
{
  
serveWebPage();   // This serves the web content that my home automation scripts scrape my sensor data.

// This is where you would also monitor sensor thresholds and fire off SMTP/SMS messages (another lab)

}


//// serveWebPage
////////////////////////////////////
void serveWebPage(){
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          sampleSensorData();
          Serial.println("Sending response");
          
          // send a standard http response header
          // use \r\n instead of many println statements for speed
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content­Type: text/html\r\n"
            "Connection: close\r\n"  // connection will be closed > response
            "Refresh: 20\r\n"        // auto­page refresh every 20 sec
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<h1>My WiFi Arduino!</h1>\r\n");
          client.print("Requests received: ");
          client.print(++reqCount);
          client.print("<br>\r\n");
          client.print(sensorWaterHeader);
          client.print(sensorWaterData);
          client.print("<br>\r\n");
          // DHT22 Humidity/Temp
          client.print(sensorDHT22HHeader);
          client.print(sensorDHT22HData);
          client.print("<br>\r\n");
          client.print(sensorDHT22THeader);
          client.print(sensorDHT22TData);
           client.print("<br>\r\n");          // PIR Motion 
          client.print(sensorPIRHeader);
          client.print(sensorPIRData);
          client.print("<br>\r\n");


          
          client.print("</html>\r\n");
          buzzer("chirp");      // See buzzer Alarms recipe if interested
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  } 
}

//// printWiFiStatus
////////////////////////////////////
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
  // make a chirp sound once on network
  buzzer("chirp"); buzzer("chirp");
}

//// sampleSensorData()
///////////////////////////////////
//// put your sensor polls in here, and separate functions below this block.
void sampleSensorData(){

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
  Serial.print("HUMIDITY: "); Serial.print(h); Serial.print(" %\t"); 
  Serial.print("Temperature: "); Serial.print(f); Serial.println(" °F");
  sensorDHT22HData = h;
  sensorDHT22TData = f;
  
  /////////// PIR Motion Sensor Code Block
  ////////////////////////////////////////////////////
  if(digitalRead(pirPin) == HIGH){              // If the PIR is HIGH
     digitalWrite(ledPin, HIGH);                // then turn on the LED pin and
     Serial.println("MOTION: HIGH (motion)");   // print "HIGH (motion)" on serial port
     sensorPIRData[15] = "Yes Motion!";
  }
  else {
     digitalWrite(ledPin, LOW);                 // else, make the LED pin low and
     Serial.println("MOTION: LOW (no motion)"); // print "LOW (no motion)" on serial port
     sensorPIRData[15] = "no motion";
  }

  ////////// Water Sensor Code Block
  ////////////////////////////////////////////////////
  waterVal = analogRead(waterPin);
  sensorWaterData = waterVal;
  Serial.print("WATER: ");Serial.println(waterVal);
  Serial.println("=========================");
  //buzzer("chirp");
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


