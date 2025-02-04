#include <SPI.h>
#include <MFRC522.h>
#include "RTClib.h"
#include "Servo.h"

#define SS_PIN 10 
#define RST_PIN 9 

MFRC522 mfrc522(SS_PIN, RST_PIN);   
RTC_DS3231 rtc;
Servo servoku;

void setup() {
  Serial.begin(9600);
  SPI.begin();      
  mfrc522.PCD_Init();
  Serial.println("Dekatkan Tag"); 
  Serial.println();

  if (!rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan!");
    while (1);
  }

  // Hanya set tanggal & waktu jika perlu, hapus setelahnya
  // rtc.adjust(DateTime(2021, 6, 17, 22, 0, 0));

  servoku.attach(9);
  pinMode(3, OUTPUT); 

  digitalWrite(3, HIGH);
  delay(300);
  digitalWrite(3, LOW);         
  servoku.write(0);       
  delay(3000);
  servoku.write(90);
}  

void loop() {
  DateTime now = rtc.now();

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag : ");
  String content = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  
  Serial.print("Message : ");
  content.toUpperCase();

  if ((content.substring(1) == "88 3A 10 4D" && now.hour() < 22 && now.hour() > 4) || 
      (content.substring(1) == "47 40 EB D7" && (now.hour() < 4 || now.hour() > 22))) {
    
    Serial.print(rtc.dayOfTheWeek());
    Serial.print(" ");  
    Serial.print(now.day(), DEC);
    Serial.print("/");
    Serial.print(now.month(), DEC);
    Serial.print("/");
    Serial.print(now.year(), DEC);
    Serial.print(" -- ");
    Serial.println(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.println(now.second(), DEC);
    
    delay(1000); 
  }
}
