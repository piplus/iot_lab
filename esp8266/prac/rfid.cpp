#include <Arduino.h>
#include <SPI.h>
#include "MFRC522.h"

#define RST_PIN  D3
#define SS_PIN  D4


MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid_read();

const int WAIT = 0;
const int TOUCH = 1;
int state;

String rfid_in = "";

void setup() {
  state = 0;
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("");
}
void loop() {
  if(state == WAIT){
    Serial.print(".");
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      state = TOUCH;
      Serial.println("Card found");
    }
    delay(1000);
  }else if(state == TOUCH){
    rfid_in=rfid_read();
    Serial.println("HEX: " + rfid_in);
    delay(1000);
    state = WAIT;
  }
}

// if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
//     rfid_in=rfid_read();
//     Serial.println("Card found");
//     Serial.println("HEX: " + rfid_in);
//     delay(1000);
//   }
//   delay(1);
String rfid_read() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}
