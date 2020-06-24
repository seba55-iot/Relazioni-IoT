#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);
const int TEMP_PIN = A1;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Temperature:");
  pinMode(TEMP_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sig = analogRead(TEMP_PIN);
  float R = ((1023.0/(float)sig) - 1.0)*R0;
  float log_sig = log(R/R0);
  float T = 1/((log_sig/B) + (1/298.15));
  float temp = T - 273.15;
  lcd.setCursor(12,0); //minimizzo la quantità di dati inviati
  lcd.print(String(temp));
  delay(10000);
}
