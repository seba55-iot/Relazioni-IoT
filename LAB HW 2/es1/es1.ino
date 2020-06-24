#include <math.h>
const int TEMP_PIN = A1;
const int FAN_PIN = 6;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;
float current_speed = 0;
const float tacca=  51.0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 2.1 Starting");
  pinMode(TEMP_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, (int) current_speed);
}

void loop() {
  int sig = analogRead(TEMP_PIN);
  float R = ((1023.0/(float)sig) - 1.0)*R0;
  float log_sig = log(R/R0);
  float T = 1/((log_sig/B) + (1/298.15));
  float temp = T - 273.15;
  Serial.println("Temperature now: " + String(temp));

  if (temp>25.0 and temp<=30.0){
    int n = temp - 25.0;
    current_speed = n*tacca;
    analogWrite(FAN_PIN, (int) current_speed);
  }
  else if (temp>30.0){
    current_speed = 255;
    analogWrite(FAN_PIN, (int) current_speed);
  }
  else{
    current_speed = 0;
    analogWrite(FAN_PIN, (int) current_speed);
  }
  delay(3000);
}
