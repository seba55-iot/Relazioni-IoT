#include <TimerOne.h>
const int SND_PIN = 7;
int n_sound_events = 2;
volatile int tot_count = 0;
long sound_interval = 10000;
volatile unsigned long start_millis = 0;

void checkPresence() {
  int value = digitalRead(SND_PIN);
  if (value == LOW) {
    Serial.println("Person Detected by Noise Sensor"); //stampa di verifica
    if (start_millis - millis() >= 2000) { //NB: start_millis a questo punto 
      tot_count += 1;                       //è ancora il valore precedente
    } //distinguo due rumori solo se distanziati di 2 sec.
    start_millis = millis();
  }
}

void setup() {
  pinMode(SND_PIN, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Lab 2.4 Starting");
  attachInterrupt(digitalPinToInterrupt(SND_PIN), checkPresence, CHANGE);
}

void loop() {
  unsigned long now_millis = millis();
  if (now_millis - start_millis >= sound_interval) { //se è trascorso sound_interval
    //dall'ultimo rilevamento, dipende tutto da quanti rumori sono stati rilevati
    if (tot_count < n_sound_events) {
      Serial.println("Non ci sono ancora persone nella stanza");
    }
    tot_count = 0;
  }
  else{
    if (tot_count >= n_sound_events) {
      Serial.println("Ci sono ancora persone nella stanza");
    }    
  }
  delay(1000);
}
