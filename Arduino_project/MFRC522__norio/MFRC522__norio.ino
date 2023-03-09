#include <SPI.h>
#include "MFRC522_I2C.h"
#include <Wire.h>

#define SCL_PIN 22
#define RFID_ADDR 0x28

//インスタンス化
MFRC522_I2C mfrc522(RFID_ADDR, SCL_PIN);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  mfrc522.PCD_Init();
  delay(4);
}

void loop() {
  if (ICRead()) {
    Serial.println("解錠します");
  }
}


//関数
boolean ICRead() {
  boolean isReadRFID = false;

  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("カードがありません");
    delay(500);
    
  } else if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("読めませんでした");
    delay(500);

  } else {
    Serial.println("読み込みました");
    String hexString = convertHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(hexString);
    if (hexString.equals("02e864d1500a70")) {//ナカモトの楽天カード
      isReadRFID = true;
      delay (1000);
    }
  }
  return isReadRFID;
}


String convertHex(byte *buffer, byte bufferSize) {
  String hexstring = "";
  for (byte i = 0; i < bufferSize; i++) {
    if (buffer[i] < 0x10) {
      hexstring += '0';
    }
    hexstring += String(buffer[i], HEX);
  }
  return hexstring;
}
