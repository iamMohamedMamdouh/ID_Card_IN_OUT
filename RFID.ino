#include <Wire.h>
#include "DFRobot_LCD.h"

 const int colorR = 255;
 const int colorG = 0;
 const int colorB = 0;

int buzzer = 11;
int greenLed = 13;
int redLed = 12;

DFRobot_LCD lcd(16,2);  

String id = "";
int count = 0;
char c;

void setup() {
  Serial.begin(9600);

  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.init(); 
  lcd.setRGB(colorR, colorG, colorB);


  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");

  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(buzzer, LOW);
}

void loop() {
  while (Serial.available() > 0) {
    c = Serial.read();
    if (c != '\n' && c != '\r') { 
      id += c;
      count++;
    }

    if (count == 12) {
      Serial.println("ID Received: " + id);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.print(id);

      if (id == "AB123456789A") { //AB1234567892
        lcd.setCursor(0, 1);
        lcd.print("Access Granted");
        digitalWrite(greenLed, HIGH);
        digitalWrite(redLed, LOW);
        digitalWrite(buzzer, HIGH);
        tone(8,1200,200);
        delay(220);
        tone(8,1200,200);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Access Denied");
        digitalWrite(greenLed, LOW);
        digitalWrite(redLed, HIGH);
        digitalWrite(buzzer, HIGH);
            tone(8,1300,800);
      }

      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scan RFID Card");

      
      id = "";
      count = 0;
      while (Serial.available() > 0) Serial.read(); 
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, LOW);
      digitalWrite(buzzer, LOW);
    }
  }
}