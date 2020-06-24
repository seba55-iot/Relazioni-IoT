#include "MQTTclient.h"
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <ArduinoJson.h>
#include <Process.h>


const int capacity1 = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + 40;
const int capacity2 = JSON_OBJECT_SIZE(3) + 40;
DynamicJsonDocument doc_snd(capacity1);
DynamicJsonDocument doc_rec(capacity1);
DynamicJsonDocument message(capacity2);
const int LED_PIN = 12;
String output = "";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 3.3 SW Starting");
  Bridge.begin();
  digitalWrite(LED_PIN,HIGH);
  mqtt.begin("test.mosquitto.org", 1883);
  //SOTTOSCRIZIONE AL CATALOG
  message["deviceID"] = 123;
  message["resources"] = "led";
  message["end-points"] = "tiot/6/device/led"; 
  serializeJson(message, output);
  mqtt.publish("tiot/6/device", output);
  mqtt.subscribe("tiot/6/device/led", setLedValue);
}

void loop() {
  mqtt.monitor();
  
  delay(50);
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

void setLedValue(const String& topic, const String& subtopic, const String& message){

  DeserializationError err = deserializeJson(doc_rec, message);
  if(err){
    Serial.println(F("DeserializedJson() failed with code "));
    Serial.println(err.c_str());
  }
  //controllare formato doc_rec
  if(doc_rec["e"][0]["n"] == "led"){
    if(doc_rec["e"][0]["v"] == 1 || doc_rec["e"][0]["v"] == 0){
      digitalWrite(LED_PIN, doc_rec["e"][0]["v"]);
    }

  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: "); 
  Serial.println(message); 
  }
}
