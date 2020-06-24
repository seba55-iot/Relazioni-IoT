#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);
const int PIR_PIN = 7;
const int SND_PIN = 8;
const int TEMP_PIN = A1;
const int FAN_PIN = 6;
const int RLED_PIN = 11;
int tot_count = 0;
int n_sound_events = 50; //50 eventi
const int timeout_pir = 1800000; //30 minuti
const long sound_interval = 600000; //10 minuti
volatile unsigned long start_millis1 = 0;
unsigned long start_millis2 = 0;
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
unsigned long now_millis = 0;
int num[4];
boolean pres;

void checkPresence() {
  int value = digitalRead(PIR_PIN);
  if (value == HIGH) {
    Serial.println("Person Detected by PIR Sensor");
    start_millis1 = millis(); //Setto il valore di start_millis ogni qual volta viene rilevato un movimento. Se per i prossimi
    //30 minuti non rilevo alcun movimento allora posso assumere che non c'è nessuno nella stanza.
    //Quanto appena descritto viene realizzato attraverso il confronto tra now_millis e start_millis1 nella loop().
    //Discorso analogo per il sensore di rumore.
  }
}

void inserisciValoriDaSeriale() {
  int i = 0;
  for (i = 0; i < 4; i++) {
    num[i] = Serial.parseInt();
    Serial.println(num[i]);
  }
  temp_min_led = num[0];
  temp_max_led = num[1];
  temp_min_dc = num[2];
  temp_max_dc = num[3];
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
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Avvio...");
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Lab 2.7 Starting");
  Serial.println("Se vuoi aggiornare i 4 set-point");
  Serial.println("Scrivi i 4 set-point nel formato: num1(temp_min_led),num2(temp_max_led),num3(temp_min_dc),num4(temp_max_dc)");
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


  //DC MOTOR
  dcMotor(temp);

  //LED
  led(temp);

  //SENSORE DI RUMORE
  int value = digitalRead(SND_PIN);
  if (value == LOW) {
    Serial.println("Person Detected by Noise Sensor"); //stampa di verifica
    if (start_millis2 - millis() >= 5000) { //NB: start_millis2 a questo punto è ancora il valore precedente
      //incremento di un'unità il numero di rumori avvertiti soltanto se sono
      //trascorsi 5 secondi tra questo rumore e il precedente.
      //L'alternativa sarebbe gestire il sensore di rumore con Interrupt e abilitare e disabilitare l'interrupt
      //con una cadenza temporale, tale da permettere di riconoscere dei rumori solo dopo x secondi dall'ultimo.
      tot_count += 1;
    }
    start_millis2 = millis();
  }


  //TIMER


  //Casi:
  //1. Ci sono persone che non inseriscono set-point -> vengono assegnati i 4 di default.
  //2. Ci sono persone che inseriscono i set-point -> vengono assegnati questi
  //3. Non ci sono persone -> vengono assegnati quelli sotto.
  //4. Ci sono persone, escono tutte e rientrano -> devono risettare i set-point altrimenti rimangono quelli
  //del punto 3.
  now_millis = millis();
  if (now_millis - start_millis1 >= timeout_pir) { //NESSUNO SI MUOVE
    if (now_millis - start_millis2 >= sound_interval) { //NESSUNO PARLA
      if (tot_count < n_sound_events) { //ANCHE SE C'E' STATO UN RUMORE SPORADICO
        Serial.println("Non ci sono persone nella stanza"); //stampa di verifica
        pres = false;
        temp_max_dc = 30;
        temp_min_dc = 25;
        temp_max_led = 25;
        temp_min_led = 20; //Con questi valori di temperatura: SE NON CI SONO PERSONE IL MOTORE è FERMO E IL LED ACCESO
      }
      tot_count = 0; //Risetto il valore dei rumori contati, al fine di portarmi dietro il conteggio di rumori sporadici.
    }
    else { //QUALCUNO PARLA
      if (tot_count >= n_sound_events) { //PARLA ABBASTANZA

        //Per evitare quanto descritto al punto 4. Qui controllo se il valore pres == false.
        //In caso affermativo la stanza era precedentemente vuota e quindi setto i valori di default della stanza piena.
        //Se poi i valori vengono settati da Seriale allora si considereranno questi.
        if (pres == false) {
          temp_max_dc = 25;
          temp_min_dc = 20;
          temp_max_led = 20;
          temp_min_led = 15;
        }
        Serial.println("Ci sono persone nella stanza");
        pres = true;

        if (Serial.available() != 0) { //SETTA I 4 SET-POINT
          inserisciValoriDaSeriale();
        }
      }
    }

  }
  else { //QUALCUNO SI MUOVE
    if (start_millis1 != 0) {
      if (pres == false) { //Con questi valori di temperatura: SE CI SONO PERSONE IL LED è SPENTO E IL MOTORE GIRA
        temp_max_dc = 25;
        temp_min_dc = 20;
        temp_max_led = 20;
        temp_min_led = 15;
      }
      Serial.println("Ci sono persone nella stanza");
      pres = true;
      if (Serial.available() != 0) { //SETTA I 4 SET-POINT
        inserisciValoriDaSeriale();
      }
    }
  }


  //LCD
  lcd.setCursor(0, 0);
  lcd.print("T:" + String(temp));
  lcd.setCursor(7, 0);
  if (pres == true) {
    lcd.print(" Pres:1");
  }
  else {
    lcd.print(" Pres:0");
  }
  lcd.setCursor(0, 1);
  lcd.print("AC:" + String((int)current_speed * 100 / 255) + "% ");
  lcd.setCursor(8, 1);
  lcd.print("HT:" + String(brightness * 100 / 255) + "%");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AC m:" + String(temp_min_dc));
  lcd.setCursor(10, 0);
  lcd.print("M:" + String(temp_max_dc));
  lcd.setCursor(0, 1);
  lcd.print("HT m:" + String(temp_min_led));
  lcd.setCursor(10, 1);
  lcd.print("M:" + String(temp_max_led));

  delay(2000);
  lcd.clear();
}
