const int PIR_PIN = 7;
const int SND_PIN = 8;
volatile int tot_count = 0;
int n_sound_events = 50; //50 eventi
const int timeout_pir = 1800000; //30 minuti
const long sound_interval = 600000; //10 minuti
volatile unsigned long start_millis1 = 0;
unsigned long start_millis2 = 0;


void checkPresence() {
  int value = digitalRead(PIR_PIN);
  if (value == HIGH) {
    Serial.println("Person Detected by PIR Sensor");
    start_millis1 = millis();
    Serial.println(start_millis1);
  }
}

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(SND_PIN, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Lab 2.5 Starting");
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), checkPresence, CHANGE);
}

void loop() {
  int value = digitalRead(SND_PIN);
  if (value == LOW) {
    Serial.println("Person Detected by Noise Sensor"); //stampa di verifica
    if (start_millis2 - millis() >= 5000) { //NB: start_millis2 a questo punto è ancora il valore precedente
      tot_count += 1;
    } //distinguo due rumori solo se distanziati di 5 sec.
    start_millis2 = millis();
  }

  unsigned long now_millis = millis();
  if (now_millis - start_millis1 >= timeout_pir) { //NESSUNO SI MUOVE
    if (now_millis - start_millis2 >= sound_interval) { //NESSUNO PARLA
      if (tot_count < n_sound_events) { //ANCHE SE C'E' STATO UN RUMORE SPORADICO
        Serial.println("Non ci sono persone nella stanza");
      }
      tot_count = 0;
    }
    else { //QUALCUNO PARLA
      if (tot_count >= n_sound_events) { // E PARLA ABBASTANZA
        Serial.println("Ci sono persone nella stanza");
      }
    }
  }

  else { //QUALCUNO SI MUOVE
    if (start_millis1 != 0) {
      Serial.println("Ci sono persone nella stanza");
    }
  }

  delay(1000);

}
