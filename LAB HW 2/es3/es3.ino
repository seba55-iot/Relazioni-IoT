const int PIR_PIN = 7;
const int timeout_pir = 30000*60;
unsigned long start_millis = 0;


void checkPresence(){
  int value = digitalRead(PIR_PIN);
  if (value == HIGH){
    Serial.println("Person Detected");
    start_millis = millis(); 
    
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIR_PIN, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 2.3 Starting"); 
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), checkPresence, CHANGE);
}

void loop() {
    unsigned long now_millis = millis();
    if(now_millis - start_millis >= timeout_pir){ 
      Serial.println("Non ci sono più persone nella stanza");
    }
    else{
      Serial.println("Ci sono ancora persone nella stanza");
    }
    delay(1000);
 
}
