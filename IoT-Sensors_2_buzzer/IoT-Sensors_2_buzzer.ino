const int buzzerPin = 3;           // Sound Buzzer on pin#

void setup() {
  pinMode(buzzerPin, OUTPUT);  
  digitalWrite(buzzerPin, HIGH); 
// Keeps buzzer from overheating and
//  making background noises.  
}

//// LOOP ////
void loop() {
  buzzer("chirp"); delay(5000);
  buzzer("success"); delay(5000);
  buzzer("fail"); delay(5000);
  buzzer("alarm"); buzzer("alarm"); delay(5000);
}

//// buzzer("success" || "fail" || "chirp" || "alarm" ) ////
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
