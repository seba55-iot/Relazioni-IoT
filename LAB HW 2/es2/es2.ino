#include <math.h>
const int TEMP_PIN = A1;
const int RLED_PIN = 11;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;
int brightness = 0;
const float tacca =  51.0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 2.2 Starting");
  pinMode(TEMP_PIN, INPUT);
  pinMode(RLED_PIN, OUTPUT);
  analogWrite(RLED_PIN, (int)brightness); 
}

void loop() {
  int sig = analogRead(TEMP_PIN);
  float R = ((1023.0/(float)sig) - 1.0)*R0;
  float log_sig = log(R/R0);
  float T = 1/((log_sig/B) + (1/298.15));
  float temp = T - 273.15;
  Serial.println("Temperature now: " + String(temp));

  if (temp>=15.0 and temp<=20.0){
    int n = 20.0 - temp;
    brightness = n*tacca;
    analogWrite(RLED_PIN, brightness);
  }
  else if (temp<15){
    brightness = 255;
    analogWrite(RLED_PIN, brightness);
  }
  else{
    brightness = 0;
    analogWrite(RLED_PIN, brightness);
  }
  Serial.println("Brightness: " + String(brightness));
  delay(3000);
}
