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
char ssid[] = "class-essid";        // the network SSID (name) you're connecting to
char pass[] = "sharedpasswd";       // the network password (blank if none/open)
int status = WL_IDLE_STATUS;            // the Wifi radio's status
int reqCount = 0;                       // number of requests received
WiFiEspServer server(80);
const int buzzerPin = 3;          // Sound Buzzer

// See Motion Sensor recipe
char sensorMotionHeader[]= "Motion State = ";
int sensorMotionData = 0;

// This added for Water Sensor
char sensorWaterHeader[]= "Water Level (0-1023) = ";
int sensorWaterData = 0;

// See DHT­22 recipe to make work
char sensorTempHeader[] = "Current Temperature(°F) = ";
float sensorTempData = 0;
// See DHT­22 recipe
char sensorHumidHeader[]= "Current Humidity(%) = ";
float sensorHumidData = 0;


///////// Setup 
////////////////////////////////
void setup()
{
  pinMode(buzzerPin, OUTPUT);           // Sets buzzerPin as output
  digitalWrite(buzzerPin, HIGH);
  ////// Setup WiFi
  Serial.begin(9600);       // initialize serial for debugging

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
    //buzzer("fail");         // hmmm.. this would be useful if it worked.
    while (true);             // don't continue
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);    // Connect to WPA/WPA2 network
  }
  tone(buzzerPin, 2000, 50); delay(50); noTone(buzzerPin);  // Connected
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
          // put more sensors here
          client.print("</html>\r\n");
          //buzzer("chirp");      // See buzzer Alarms recipe if interested
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
  tone(buzzerPin, 2000, 50); delay(50); noTone(buzzerPin);
}

//// sampleSensorData()
///////////////////////////////////
//// put your sensor polls in here, and separate functions below this block.
void sampleSensorData(){
  // This is where you do your light, temperature and humidity sampling (DHT­22 could take TWO SECONDS)
  // Water Sensor 
  sensorWaterData = analogRead(A0);
  // DHT­22 Humidity/Temp Sensor (see recipe for more info)
  // Other sensors to sample?
    tone(buzzerPin, 1000, 50); delay(50); noTone(buzzerPin);
}

