//Water Sensor variables:
const int waterPin = A0;
int waterVal = 0;

void setup() {
  Serial.begin(9600);
}

void loop(){
  delay(1000);
  waterVal = analogRead(waterPin);
  Serial.print("WATER: ");Serial.println(waterVal);
}
