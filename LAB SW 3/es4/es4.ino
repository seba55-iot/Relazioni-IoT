#include <LiquidCrystal_PCF8574.h>
#include "MQTTclient.h"
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <ArduinoJson.h>
#include <Process.h>

const int capacity1 = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + 40;
const int capacity2 = JSON_OBJECT_SIZE(3) + 56;
DynamicJsonDocument doc_snd(capacity1);
//DynamicJsonDocument doc_rec(capacity1);
DynamicJsonDocument message(capacity2);
LiquidCrystal_PCF8574 lcd(0x27);
const int PIR_PIN = 7;
const int SND_PIN = 8;
const int TEMP_PIN = A1;
const int FAN_PIN = 6;
const int RLED_PIN = 12;
int tot_count = 0;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;
float current_speed = 0;
int brightness = 0;
const float tacca =  51.0;
float temp_max_dc = 25;
float temp_min_dc = 20;
float temp_max_led = 20;
float temp_min_led = 15;
//unsigned long now_millis = 0;
boolean pres;


void checkPresence() {
  int value = digitalRead(PIR_PIN);
  if (value == HIGH) {
    Serial.println("Person Detected by PIR Sensor");
    String message = senMLEncode("presence", 1, "");
    mqtt.publish("tiot/6/device/sensors", message);
  }

}

void led(float temp) {
  if (temp >= temp_min_led and temp <= temp_max_led) {
    int n = temp_max_led - temp;
    brightness = n * tacca;
    analogWrite(RLED_PIN, brightness);
  }

  else if (temp < temp_min_led) {
    analogWrite(RLED_PIN, 255);
    brightness = 255;
  }
  else {
    analogWrite(RLED_PIN, 0);
    brightness = 0;
  }
}

void dcMotor(float temp) {
  if (temp > temp_min_dc and temp <= temp_max_dc) {
    int n = temp - temp_min_dc;
    current_speed = n * tacca;
    analogWrite(FAN_PIN, (int) current_speed);
  }

  else if (temp > temp_max_dc) {
    current_speed = 255;
    analogWrite(FAN_PIN, (int) current_speed);
  }
  else {
    current_speed = 0;
    analogWrite(FAN_PIN, (int) current_speed);
  }
}

String senMLEncode(String res, float v, String unit){
  doc_snd.clear();
  doc_snd["bn"] = "Yun";
  if (unit!=""){
    doc_snd["e"][0]["u"] = unit;
  }
  else{
    doc_snd["e"][0]["u"] = (char*)NULL;
  }
  
  if (String(v)!=""){
    doc_snd["e"][0]["v"] = v;
  }
  else{
    doc_snd["e"][0]["v"] = (char*)NULL;
  }

  if (res!=""){
    doc_snd["e"][0]["n"] = res;
  }
  else{
    doc_snd["e"][0]["n"] = (char*)NULL;
  }

  doc_snd["e"][0]["t"] = String(millis()/1000.0) + "s";

  String output;
  serializeJson(doc_snd, output);
  return output;
}

void setValue(const String& topic, const String& subtopic, const String& message){

  DeserializationError err = deserializeJson(doc_snd, message);
  if(err){
    Serial.println(F("DeserializedJson() failed with code "));
    Serial.println(err.c_str());
  }

  if(doc_snd["e"][0]["n"] == "led"){
      led(doc_snd["e"][0]["v"]); //doc_rec = val di temperatura
    }
  if(doc_snd["e"][0]["n"] == "lcd"){
    lcd.clear();
    lcd.print(doc_snd["e"][0]["v"].as<String>());
    }
    
   if(doc_snd["e"][0]["n"] == "temp"){
    dcMotor(doc_snd["e"][0]["v"]);
    }
  
 }


void setup() {
  // put your setup code here, to run once:
  pinMode(PIR_PIN, INPUT);
  pinMode(SND_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  
  pinMode(FAN_PIN, OUTPUT);
  pinMode(RLED_PIN, OUTPUT);
  digitalWrite(RLED_PIN, LOW);
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Avvio...");
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 3.4 SW Starting");
  Bridge.begin();
  digitalWrite(RLED_PIN,HIGH);
  mqtt.begin("test.mosquitto.org", 1883);
  
  //SOTTOSCRIZIONE AL CATALOG
  message.clear();
  message["deviceID"] = 123;
  message["resources"] = "led";
  message["end-points"] = "tiot/6/device/sensors"; 
  String output;
 serializeJson(message, output);
 Serial.println(output);
  
  mqtt.publish("tiot/6/device", output);
  mqtt.subscribe("tiot/6/device/sensors", setValue);
  analogWrite(FAN_PIN, (int)current_speed);
  analogWrite(RLED_PIN, (int)brightness);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), checkPresence, CHANGE);
}

void loop() {

  int sig = analogRead(TEMP_PIN);
  float R = ((1023.0 / (float)sig) - 1.0) * R0;
  float log_sig = log(R / R0);
  float T = 1 / ((log_sig / B) + (1 / 298.15));
  float temp = T - 273.15;
  Serial.println("Temperature now: " + String(temp)); //stampa di verifica
  String message = senMLEncode("temperature", temp, "Cel");
  mqtt.publish("tiot/6/device/sensors", message);
  Serial.println(message);


 //SENSORE DI RUMORE
  int value = digitalRead(SND_PIN);
  if (value == LOW) {
      String message = senMLEncode("presence", 1, "");
      mqtt.publish("tiot/6/device/sensors", message);
  }

delay(2000);
}
