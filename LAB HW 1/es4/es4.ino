float current_speed = 0;
const int FAN_PIN = 6;

void setup() {
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, (int) current_speed);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Lab 1.4 Starting");
}

void loop() {
  if(Serial.available()>0){
    int val = Serial.read();
    if (val == '+'){
      current_speed+=25.5;
      if (current_speed <= 255){
       Serial.println("Increasing speed: ");
       Serial.println(current_speed);
       analogWrite(FAN_PIN, (int) current_speed);
      }
      else{
        Serial.println("Already at max speed");
      }
    }
     else if (val == '-'){
      if (current_speed == 0){
         Serial.println("Already at min speed");
      }
      else{
       Serial.println("Decreasing speed: ");
       Serial.println(current_speed);
      current_speed-=25.5;
      analogWrite(FAN_PIN, (int) current_speed);
      }
    }
    else{
      Serial.println("Valore non riconosciuto");
    }
  }
}
