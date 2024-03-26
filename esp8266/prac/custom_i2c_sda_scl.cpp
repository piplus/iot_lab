//https://github.com/thanasinb/IoT-Learning-Set/tree/main/%E0%B8%9A%E0%B8%97%E0%B8%97%E0%B8%B5%E0%B9%888

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Wire.begin(D4,D3);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(115200);
  lcd.setCursor(0,0);
  lcd.print("Hello World!!");
}

void loop()
{
  
}