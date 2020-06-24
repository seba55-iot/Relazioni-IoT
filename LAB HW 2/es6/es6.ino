const int PIR_PIN = 7;
const int SND_PIN = 8;
const int TEMP_PIN = A1;
const int FAN_PIN = 6;
const int RLED_PIN = 11;
int tot_count = 0;
int n_sound_events = 50; //50 eventi
const int timeout_pir = 1800000; //30 minuti
const long sound_interval = 600000; //10 minuti
unsigned long start_millis1 = 0;
unsigned long start_millis2 = 0;
const float B = 4275;
const float R0 = 100000;
const float T0 = 298.15;
float current_speed = 0;
int brightness = 0;
const float tacca =  51.0;
float temp_max_dc = 30;
float temp_min_dc = 25;
float temp_max_led = 20;
float temp_min_led = 15;


void checkPresence() {
  int value = digitalRead(PIR_PIN);
  if (value == HIGH) {
    Serial.println("Person Detected by PIR Sensor");
    start_millis1 = millis();

  }
}

void led(float temp) {
  if (temp >= temp_min_led and temp <= temp_max_led) {
    int n = temp_max_led - temp;
    brightness = n * tacca;
    analogWrite(RLED_PIN, brightness);
    Serial.println("Brightness:" + String(brightness)); //stampa di verifica
  }

  else if (temp < temp_min_led) {
    analogWrite(RLED_PIN, 255);
    brightness = 255;
    Serial.println("Brightness:" + String(brightness)); //stampa di verifica
  }
  else {
    analogWrite(RLED_PIN, 0);
    brightness = 0;
    Serial.println("Brightness:" + String(brightness)); //stampa di verifica
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

void setup() {
  // put your setup code here, to run once:
  pinMode(PIR_PIN, INPUT);
  pinMode(SND_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(RLED_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Lab 2.6 Starting");
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
  Serial.println("Temperature now: " + String(temp));



  //DC MOTOR
  dcMotor(temp);

  //LED
  led(temp);


  int value = digitalRead(SND_PIN);
  if (value == LOW) {
    Serial.println("Person Detected by Noise Sensor"); //stampa di verifica
    if (start_millis2 - millis() >= 5000) { //NB: start_millis2 a questo punto è ancora il valore precedente
      tot_count += 1;
    }
    start_millis2 = millis();
  }

  unsigned long now_millis = millis();
  if (now_millis - start_millis1 >= timeout_pir) { //NESSUNO SI MUOVE
    if (now_millis - start_millis2 >= sound_interval) { //NESSUNO PARLA
      if (tot_count < n_sound_events) { //ANCHE SE C'E' STATO UN RUMORE SPORADICO
        Serial.println("Non ci sono persone nella stanza");
        temp_max_dc = 35;
        temp_min_dc = 30;
        temp_max_led = 25;
        temp_min_led = 20; //SE NON CI SONO PERSONE IL MOTORE è FERMO E IL LED ACCESO
      }
      tot_count = 0;
    }
    else { //QUALCUNO PARLA
      Serial.println(tot_count);
      if (tot_count >= n_sound_events) { //PARLA ABBASTANZA
        Serial.println("Ci sono persone nella stanza");

        temp_max_dc = 20;
        temp_min_dc = 15;
        temp_max_led = 20;
        temp_min_led = 15;

      }
    }
  }

    else { //QUALCUNO SI MUOVE
      if (start_millis1 != 0) {
        Serial.println("Ci sono persone nella stanza");
        temp_max_dc = 20;
        temp_min_dc = 15;
        temp_max_led = 20;
        temp_min_led = 15;
        //SE CI SONO PERSONE IL LED è SPENTO E IL MOTORE GIRA
      }
    }

    delay(1000);

  }
