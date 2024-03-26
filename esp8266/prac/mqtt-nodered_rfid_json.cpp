#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "MFRC522.h"
#include <ArduinoJson.h>

#define WIFI_STA_NAME "honorone" // ชื่อ wifi
#define WIFI_STA_PASS "77777777" // รหัส wifi
#define MQTT_SERVER "174.138.17.96" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883 // Port MQTT Broker
#define MQTT_USERNAME "emqx"
#define MQTT_PASSWORD "public"
#define MQTT_NAME "pppp" // ชื่อที่ต้องการให้แสดงใน MQTT Broker
#define MQTT_TOPIC_SEND "prac/door/send"
#define MQTT_TOPIC_RECV "prac/door/recv" // ชื่อ Topic

// rfid
#define RST_PIN  D3
#define SS_PIN  D4

MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid_read();

String rfid_in = "";

//




void initWiFi();

void callback(char *topic, byte *payload, unsigned int length);
WiFiClient client;
PubSubClient MQTT(client);
bool message_flag;
String message;


const int WIFI = 0;
const int MQTT_CHECK = 1;
const int SEND = 2;
const int RECV = 3;
int state ;

void setup() {
  state = 0;
  pinMode(D0,OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.begin(115200);
  initWiFi();
  MQTT.setServer(MQTT_SERVER, MQTT_PORT); // กำห น ด MQTT Server และ Port
  MQTT.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD); 
}

void loop() {
  if(state == WIFI){
    Serial.println("Check Wifi");
    if(WiFi.status() != WL_CONNECTED){
      initWiFi();
    }else{
      state = MQTT_CHECK;
    }
  }else if(state == MQTT_CHECK){
    Serial.println("Check MQTT");
    if(MQTT.connected()){
      state = SEND;
      MQTT.setCallback(callback); // กำหนดฟัง ก ์ ชั น callback เป็นฟังก์ชันรับข้อมูลจาก MQTT Broker
      MQTT.subscribe(MQTT_TOPIC_RECV);
      Serial.println("MQTT_CONNECTED");
    }else{
      Serial.println("MQTT_UNCONNECTED"); 
    }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI;
    }
    delay(1000);
  }else if(state == SEND){
    Serial.print("*");
    MQTT.loop();

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      Serial.println("Card found");
      rfid_in=rfid_read();
      Serial.println(rfid_in);
      char strcard[rfid_in.length()+1];
      rfid_in.toCharArray(strcard,rfid_in.length()+1);
      if(MQTT.publish(MQTT_TOPIC_SEND,strcard) == true){
        Serial.println("Success sending");
        state = RECV;
      }
    }
    // if(MQTT.publish(MQTT_TOPIC_SEND,"Pornpipat") == true){
    //   Serial.println("Success sending");
    //   state = RECV;
    // }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI;
    }
    delay(1000);
  }else if(state == RECV){
    MQTT.loop();
    if(message_flag == true){
      JsonDocument doc;
      deserializeJson(doc, message);
      if(doc["door"] == "open"){
        digitalWrite(D0,LOW);
        delay(3000);
        digitalWrite(D0,HIGH);
        delay(100);
      }
      Serial.println(message);
      message_flag = false;
      state = SEND;
    }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI;
    }
    delay(4000);
  }
}

void callback(char *topic, byte *payload, unsigned int length){// ฟังก์ชันสำหรับรับข้อมูลจาก MQTT Broker{
    message = ""; // ล้างข้อมูลที่เก็บไว้ในตัวแปร message
    message_flag = true; // กำหนดค่า message_flag เป็น true เพื่อบอกให้รู้ข้อมูลใหม่เข้ามา
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] : ");
    for (int i = 0; i < length; i++){
    message += (char)payload[i]; // เก็บข้อมูลที่ได้รับไว้ในตัวแปร message
    }
}

void initWiFi(){
  WiFi.begin(WIFI_STA_NAME,WIFI_STA_PASS);
    Serial.println("Conecting ..");
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }
    Serial.println(WiFi.localIP());
}

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


