#include "MQTTclient.h"
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <ArduinoJson.h>
#include <Process.h>

const int capacity1 = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + 40;
const int capacity2 = JSON_OBJECT_SIZE(3) + 40;
DynamicJsonDocument doc_snd(capacity1);
DynamicJsonDocument message(capacity2);
const int LED_PIN = 12;
const int TEMP_PIN = A1;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;
String output;



void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TEMP_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 3.2 SW Starting");
  Bridge.begin();
  digitalWrite(LED_PIN,HIGH);
  mqtt.begin("test.mosquitto.org", 1883);
  //SOTTOSCRIZIONE
  message["deviceID"] = 123;
  message["resources"] = "temperature";
  message["end-points"] = "tiot/6/devices/temperature"; 
  serializeJson(message, output);
  mqtt.publish("tiot/6/device", output);
}

void loop() {
  mqtt.monitor();

  //Temperatura
  float temp = readTemp();
  String message = senMLEncode("temperature", temp, "Cel");
  mqtt.publish("tiot/6/devices/temperature", message);
  Serial.println(temp);
  delay(3000);

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

float readTemp(){
  int sig = analogRead(TEMP_PIN);
  float R = ((1023.0/(float)sig) - 1.0)*R0;
  float log_sig = log(R/R0);
  float T = 1/((log_sig/B) + (1/298.15));
  float temp = T - 273.15;
  return temp;
}
