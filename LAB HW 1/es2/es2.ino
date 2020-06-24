#include <TimerOne.h>
const int RLED_PIN = 12;
const int GLED_PIN = 11;
const float R_HALF_PERIOD = 1.5;
const float G_HALF_PERIOD = 3.5;

volatile int greenLedState = LOW;
volatile int redLedState = LOW;

void blinkGreen(){
  greenLedState = !greenLedState;
  digitalWrite(GLED_PIN, greenLedState);
}

void serialPrintStatus(){
  if(Serial.available() > 0){
    int inByte = Serial.read();
    if(inByte == 'R'){
      if (redLedState == LOW){
        Serial.println("Led Rosso: spento");
      }
      else {
        Serial.println("Led Rosso: acceso");
      }
    }
    else if(inByte == 'L'){
      if (greenLedState == LOW){
        Serial.println("Led Verde: spento");
      }
      else {
        Serial.println("Led Verde: acceso");
      }
    }
    else {
      Serial.println("Carattere non riconosciuto");
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RLED_PIN, OUTPUT);
  pinMode(GLED_PIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 1.2 Starting");
  Timer1.initialize(G_HALF_PERIOD *1e06);
  Timer1.attachInterrupt(blinkGreen);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  redLedState = !redLedState;
  digitalWrite(RLED_PIN, redLedState);
  serialPrintStatus();
  delay(R_HALF_PERIOD * 1e03);
  
}
