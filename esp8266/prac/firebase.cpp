//https://github.com/thanasinb/IoT-Learning-Set/tree/main/%E0%B8%9A%E0%B8%97%E0%B8%97%E0%B8%B5%E0%B9%888

#include <Arduino.h>
#include <Firebase_ESP_Client.h> // Firebase library
#include <ESP8266WiFi.h>
#include "addons/TokenHelper.h" // Firebase token generation
#include "addons/RTDBHelper.h"  // Firebase Realtime Database helper
#define WIFI_SSID "honorone" //TP-Link-A287
#define WIFI_PASSWORD "77777777" //66845355
#define API_KEY "AIzaSyBrMfYH3EZzzrYz87titANMaNhF_D4cBLM"         // API key ของ Firebase ที่ได้รับจากการสร้างโปรเจคใน Firebase Console
#define USER_EMAIL "phai@rmutt.ac.th"      // อีเมลของบัญชี Firebase ที่ใช้สำหรับเข้าสู่ระบบ
#define USER_PASSWORD "77777777"   // รหัสผ่านของบัญชี Firebase ที่ใช้สำหรับเข้าสู่ระบบ
#define DATABASE_URL "https://praciot-c51c2-default-rtdb.asia-southeast1.firebasedatabase.app/"    // URL ของ Firebase Realtime Database

FirebaseData fbdo;          // Object สำหรับเก็บข้อมูลที่ได้จาก Firebase
FirebaseAuth auth;          // Object สำหรับเก็บข้อมูลการเข้าสู่ระบบ
FirebaseConfig config;      // Object สำหรับเก็บข้อมูลการเชื่อมต่อ Firebase

String databasePath;        // ตัวแปรสำหรับเก็บค่า path ของ Firebase Realtime Database
float data;

void sendDataToFirebase(String path, float value)
{
    // ถ้าส่งค่าไปยัง Firebase สำเร็จจะแสดงข้อความ "PASSED" และแสดง path และ type ของข้อมูลที่ Firebase ส่งกลับมา
    if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value))
    {
        Serial.print("Writing value: ");
        Serial.print(value);
        Serial.print(" on the following path: ");
        Serial.println(path);
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
    }
      
    // ถ้าส่งค่าไปยัง Firebase ไม่สำเร็จจะแสดงข้อความ "FAILED" และแสดงเหตุผลที่เกิดข้อผิดพลาด
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
    }
}

void initWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    initWiFi();
    config.api_key = API_KEY;                             // ตั้งค่า API key
    auth.user.email = USER_EMAIL;                         // ตั้งค่าอีเมลของบัญชี Firebase
    auth.user.password = USER_PASSWORD;                   // ตั้งค่ารหัสผ่านของบัญชี Firebase
    config.database_url = DATABASE_URL;                   // ตั้งค่า URL ของ Firebase Realtime Database
    Firebase.reconnectWiFi(true);                         // ตั้งค่าให้ Firebase ทำการเชื่อมต่อ WiFi ใหม่เมื่อเกิดการตัดสัญญาณ
    fbdo.setResponseSize(4096);                           // ตั้งค่าขนาดของข้อมูลที่ Firebase ส่งกลับมาให้เก็บไว้ใน Object
    config.token_status_callback = tokenStatusCallback;   // ตั้งค่า callback function สำหรับการตรวจสอบสถานะของ Firebase token
    config.max_token_generation_retry = 5;                // ตั้งค่าจำนวนครั้งที่ Firebase จะทำการสร้าง token ใหม่หากเกิดข้อผิดพลาด
    Firebase.begin(&config, &auth);                       // เริ่มต้นการทำงานของ Firebase
    Serial.println("Getting User UID");
    
    // ตรวจสอบสถานะของ Firebase token ว่ามีการสร้าง token แล้วหรือยัง
    while ((auth.token.uid) == "")
    {
        Serial.print('.');
        delay(1000);
    }

    databasePath = "readings";                           // ตั้งค่า path ของ Firebase Realtime Database
    data = 0.0;
}

void loop() {
  // put your main code here, to run repeatedly:
    sendDataToFirebase(databasePath, data);             // ส่งตัวเลขไปที่ฟังก์ชัน sendDataToFirebase เพื่อส่งตัวเลขเข้า Firebase
    data = data + 1.5;
    delay(500);
}