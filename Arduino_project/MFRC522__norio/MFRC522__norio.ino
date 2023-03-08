#include <SPI.h>
#include "MFRC522_I2C.h"
#include <Wire.h>

#define SLAVE_ADDR 0x50

byte nuidPICC[4];
const char closeC[] = "lock";
const char openC[] = "open";
MFRC522_I2C mfrc522(0x28, 22);   // Create MFRC522 instance.
int onRFID = 0;  //カードがリーダーにかざされているかどうか

void setup() {
  Wire.begin();
  Serial.begin(115200);
  mfrc522.PCD_Init();
  delay(4);
  onRFID = 0;
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("カードがありません");
    if (onRFID != 0) {
      onRFID = 0;
      Wire1.beginTransmission(SLAVE_ADDR);
      for (int i = 0; i < strlen(closeC); i++) {
        Wire1.write(closeC[i]);
      }
      Wire1.endTransmission();
    } return;
  } else {
    if (!mfrc522.PICC_ReadCardSerial()) {
      Serial.println("読めませんでした");
      delay(50);
      return;
    }
    String hexString = convertHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println("読み込みました");
    Serial.println(hexString);
    if (hexString.equals("NFCカードのユニークID")) {
      if (onRFID == 1) { } else {
        onRFID = 1;
        Wire1.beginTransmission(SLAVE_ADDR);
        for (int i = 0; i < strlen(openC); i++) {
          Wire1.write(openC[i]);
        } Wire1.endTransmission();
        delay (1000);
      }
    }
  }
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

String HexString2ASCIIString(String hexstring) {
  String t = "", sub = "", r;
  char buf[3];
  for (int i = 0; i < hexstring.length(); i += 2) {
    sub = hexstring.substring(i, i + 2);
    sub.toCharArray(buf, 3);
    char b = (char)strtol(buf, 0, 16);
    if (b == '\0') break;
    t += b;
  }
  return t;
}
