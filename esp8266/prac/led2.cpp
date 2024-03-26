#include <Arduino.h>
const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;
int state;


void setup() {
  state = 0;
  pinMode(D0,INPUT);
  pinMode(D1,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println(digitalRead(D0));
  if(state == A){
    digitalWrite(D1,LOW);
    if(digitalRead(D0) == 1){
      state = B;
    }
  }else if(state == B){
    digitalWrite(D1,LOW);
    if(digitalRead(D0) == 0){
      state = C;
    }
  }else if(state == C){
    digitalWrite(D1,HIGH);
    if(digitalRead(D0) == 1){
      state = D;
    }
  }else if(state == D){
    digitalWrite(D1,HIGH);
    if(digitalRead(D0) == 0){
      state = A;
    }
  }

  delay(100);
}

