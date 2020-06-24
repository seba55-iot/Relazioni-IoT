#include <TimerOne.h>
const int RLED_PIN = 12;
const int PIR_PIN = 7;
const float R_HALF_PERIOD = 1.5;
volatile int redLedState = LOW;
volatile int tot_count = 0;


void checkPresence(){
  int value = digitalRead(PIR_PIN);
  if (value == HIGH){
    Serial.println("Motion Detected");
    tot_count +=1;
    redLedState = HIGH;
    digitalWrite(RLED_PIN, redLedState);
  }
  else{
    redLedState = LOW;
    digitalWrite(RLED_PIN, redLedState);
  }

}

void setup() {
  // put your setup code here, to run once:
  pinMode(RLED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 1.3 Starting"); 
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), checkPresence, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(30000);
  Serial.println("Total People Count: ");
  Serial.println(tot_count);
  
}
