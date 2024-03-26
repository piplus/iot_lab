#include <Arduino.h>
const int OFF = 0;
const int ON = 1;
int state;


void setup() {
  state = 0;
  pinMode(D0,INPUT);
  pinMode(D1,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println(digitalRead(D0));
  if(state == OFF){
    digitalWrite(D1,LOW);
    if(digitalRead(D0) == 1){
      state = ON;
    }
  }else if(state == ON){
    digitalWrite(D1,HIGH);
    if(digitalRead(D0) == 0){
      state = OFF;
    }
  }
  delay(100);
}

