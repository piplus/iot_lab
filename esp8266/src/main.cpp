//https://github.com/thanasinb/IoT-Learning-Set/tree/main/%E0%B8%9A%E0%B8%97%E0%B8%97%E0%B8%B5%E0%B9%888

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

LiquidCrystal_I2C lcd(0x27,20,4);

#include <SPI.h>
#include "MFRC522.h"
#define WIFI_STA_NAME "honorone" // ชื่อ wifi
#define WIFI_STA_PASS "77777777" // รหัส wifi
#define MQTT_SERVER "174.138.17.96" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883 // Port MQTT Broker
#define MQTT_USERNAME "emqx"
#define MQTT_PASSWORD "public"
#define MQTT_NAME "practo" // ชื่อที่ต้องการให้แสดงใน MQTT Broker
#define MQTT_TOPIC "prac/t1" // ชื่อ Topic


#define RST_PIN  D3
#define SS_PIN  D4

void initWiFi();

void callback(char *topic, byte *payload, unsigned int length);
WiFiClient client;
PubSubClient MQTT(client);
bool message_flag;
String message;

MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid_read();
const int WIFI_CON = 0;
const int MQTT_CON = 1;
const int SEND = 2;
const int RECV = 3;
int state = 0;




String rfid_in = "";

void setup()
{
  // Wire.begin(D4,D3);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  initWiFi();
  MQTT.setServer(MQTT_SERVER, MQTT_PORT); // กำห น ด MQTT Server และ Port
  MQTT.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD);                       // initialize the lcd 
  lcd.backlight();
  Serial.begin(115200);
  lcd.setCursor(0,0);
  lcd.print("Hello World!!");
  MQTT.setCallback(callback); // กำหนดฟัง ก ์ ชั น callback เป็นฟังก์ชันรับข้อมูลจาก MQTT Broker
  MQTT.subscribe(MQTT_TOPIC);
}

void loop()
{
  if(state == WIFI_CON){
    if(WiFi.status() == WL_CONNECTED){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("WIFI CONECTED");
      delay(1000);
      state = MQTT_CON;
    }else{
      initWiFi();
    }
  }else if(state == MQTT_CON){
    Serial.println("Check MQTT");
    if(MQTT.connected()){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("MQTT CONECTED");
      delay(1000);
      state = SEND;
      Serial.println("MQTT_CONNECTED");
    }else{
      Serial.println("MQTT_UNCONNECTED"); 
    }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI_CON;
    }
  }else if(state == SEND){
    MQTT.loop();
    if(MQTT.publish(MQTT_TOPIC,"LCD SENDING") == true){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("SENDING SUCCESS");
      delay(1000);
      state = RECV;
    }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI_CON;
    }
    delay(2000);
  }else if(state == RECV){
    MQTT.loop();
    if(message_flag == true){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(message);
      message_flag = false;
      state = SEND;
    }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI_CON;
    }
    delay(5000);
  }
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