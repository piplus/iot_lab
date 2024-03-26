#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define WIFI_STA_NAME "honorone" // ชื่อ wifi
#define WIFI_STA_PASS "77777777" // รหัส wifi
#define MQTT_SERVER "174.138.17.96" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883 // Port MQTT Broker
#define MQTT_USERNAME "emqx"
#define MQTT_PASSWORD "public"
#define MQTT_NAME "practo" // ชื่อที่ต้องการให้แสดงใน MQTT Broker
#define MQTT_TOPIC "prac/t1" // ชื่อ Topic

void initWiFi();

void callback(char *topic, byte *payload, unsigned int length);
WiFiClient client;
PubSubClient MQTT(client);
bool message_flag;
String message;


const int WIFI = 0;
const int MQTT_CHECK = 1;
const int SEND = 2;
int state ;

void setup() {
  state = 0;
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
      Serial.println("MQTT_CONNECTED");
    }else{
      Serial.println("MQTT_UNCONNECTED"); 
    }
    if(WiFi.status() != WL_CONNECTED){
      state = WIFI;
    }
    delay(1000);
  }else if(state == SEND){
    Serial.println("SEND");
    delay(1000);
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


