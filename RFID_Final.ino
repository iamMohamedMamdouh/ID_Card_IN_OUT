// مكتبات
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10 //SDA
#define RST_PIN 9 //REST

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // غير العنوان لو الشاشة مش شغالة

// الكروت
byte card_ID[4];
byte Name1[4] = {0x73, 0x48, 0xC3, 0x12};
byte Name2[4] = {0x43, 0xE4, 0x3B, 0xFA};

int CardState[2] = {0, 0}; // 0 = أول مرة (دخول)، 1 = تاني مرة (خروج)
int j = 0;

int const RedLed = 6;
int const GreenLed = 5;
int const Buzzer = 8;

String Name;
long Number;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); // بيكتب علي السطر الاول
  lcd.print("Scan your Card");

  Serial.println("CLEARSHEET");
  Serial.println("LABEL,Date,Time,Name,Number,Status");

  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  bool validCard = false;

// تعريف الكروت
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID[i] = mfrc522.uid.uidByte[i];
    if (card_ID[i] == Name1[i]) {
      Name = "Mohamed";
      Number = 123456;
      j = 0;
      validCard = true;
    } else if (card_ID[i] == Name2[i]) {
      Name = "YOSIF";
      Number = 789101;
      j = 1;
      validCard = true;
    } else {
      validCard = false;
      break;
    }
  }

// لو الكارت غلط
  if (!validCard) {
    lcd.clear(); //يمسح الشاشة
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
    digitalWrite(Buzzer, HIGH);
    delay(1000);
    digitalWrite(Buzzer, LOW);
    goto cont;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Name);

  if (CardState[j] == 0) {
    // دخول
    lcd.setCursor(0, 1);
    lcd.print("Checked IN");
    Serial.print("DATA,DATE,TIME,");
    Serial.print(Name);
    Serial.print(",");
    Serial.print(Number);
    Serial.println(",IN");

    CardState[j] = 1; // خلي الحالة خروج المرة الجاية
  } else {
    // خروج
    lcd.setCursor(0, 1);
    lcd.print("Checked OUT");
    Serial.print("DATA,DATE,TIME,");
    Serial.print(Name);
    Serial.print(",");
    Serial.print(Number);
    Serial.println(",OUT");

    CardState[j] = 0; // رجّع الحالة لدخول تاني
  }

  digitalWrite(GreenLed, HIGH);
  digitalWrite(RedLed, LOW);
  digitalWrite(Buzzer, HIGH);
  delay(30);
  digitalWrite(Buzzer, LOW);
  Serial.println("SAVEWORKBOOKAS,Names/WorkNames"); // شيت الاكسيل

  delay(2000);
cont:
  digitalWrite(GreenLed, LOW);
  digitalWrite(RedLed, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your Card");
}
