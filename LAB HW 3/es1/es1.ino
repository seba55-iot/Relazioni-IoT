#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <ArduinoJson.h>

const int LED_PIN = 12;
const int TEMP_PIN = A1;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;
BridgeServer server;
const int capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + 40;
DynamicJsonDocument doc_snd(capacity);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TEMP_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 4.1 SW starting");
  Bridge.begin();
  digitalWrite(LED_PIN,HIGH);
  
  server.listenOnLocalhost();
  server.begin();
}

void loop() { //gestire le richieste provenienti al server
   BridgeClient client = server.accept(); //accetta nuovi client provenienti verso il server
   
   if(client){
    process(client);
    client.stop();
   }
   delay(50);
}

float readTemp(){
  int sig = analogRead(TEMP_PIN);
  float R = ((1023.0/(float)sig) - 1.0)*R0;
  float log_sig = log(R/R0);
  float T = 1/((log_sig/B) + (1/298.15));
  float temp = T - 273.15;
  return temp;
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

void printResponse(BridgeClient client, int code, String body){
  client.println("Status: " + String(code));
  if (code == 200){
    client.println(F("Content-type: application/json; charset=utf-8"));
    client.println();
    client.println(body);
  }
}

void process(BridgeClient client){
  String command = client.readStringUntil('/');
  command.trim();

  if (command == "led"){
    int val = client.parseInt();
    if (val == 0 || val == 1){
      digitalWrite(LED_PIN, val);
      printResponse(client, 200, senMLEncode(F("led"), val, F(""))); //F è una macro  
                                                                
    }
    else{
      printResponse(client,404, "");
    }
  }
  else if (command == "temperature") {
    printResponse(client, 200, senMLEncode(F("Temperature"), readTemp(), F("")));
  }
  else {
    printResponse(client,400, "");    
  }
}
