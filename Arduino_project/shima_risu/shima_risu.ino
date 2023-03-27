#include "shima_risu.h"
#include "Sensor.h"
#include "Lock.h"
#include "Button.h"
#include "Led.h"
//#include "Timer.h"
#include <SPI.h>
#include "MFRC522_I2C.h"
#include <Wire.h>
#include <dummy.h>
#include <BluetoothSerial.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>  // only for esp_wifi_set_channel()


//ピン番号
#define BUTTON_PIN 15
#define SERVO_PIN 13
#define ECHO_PIN 16
#define TRIG_PIN 17
#define RED_PIN 32
#define GREEN_PIN 33
#define SCL_PIN 22


//I2C通信アドレス
#define RFID_ADDR 0x28

//ESPNOW
#define CHANNEL 1           //wi-fiのチャンネル　子機と合わせる
#define PRINTSCANRESULTS 0  //スキャン結果の表示　1：表示　0：非表示
#define DELETEBEFOREPAIR 0  //ペアリング前のスレーブ削除
#define OPEN_SENDDATA 0     //sendData()に引数として渡す
#define LOCK_SENDDATA 1     //sendData()に引数として渡す
#define BUZZER_SENDDATA 2   //sendData()に引数として渡す


//グローバル変数
int secondSensor = 3;  //onTimer(割り込み関数)の処理をするまでの秒数
int secondLock = 4;
boolean flag = true;
MODE g_mode = INIT_MODE;
ACTION g_action = SENSOR_ACTION;
esp_now_peer_info_t slave;


//インスタンス化
Lock lock = Lock(SERVO_PIN);
Sensor sensor = Sensor(ECHO_PIN, TRIG_PIN);
Button btn = Button(BUTTON_PIN);
Led ledRed = Led(RED_PIN);
Led ledGreen = Led(GREEN_PIN);
//Timer timerSensor = Timer(onTimerSensor);
//Timer timerLock = Timer(onTimerLock);
MFRC522_I2C mfrc522(RFID_ADDR, SCL_PIN);
BluetoothSerial SerialBT;


void setup() {
  Wire.begin();
  Serial.begin(115200);
  SerialBT.begin(F("ESP32_SHIMARISU_SE20"));  //Bluetooth device name
  Serial.println(F("The device started, now you can pair it with bluetooth!"));
  mfrc522.PCD_Init();
  delay(4);

  //BluetoothのMacAddressをSerial表示
  uint8_t mac5[6];
  esp_read_mac(mac5, ESP_MAC_BT);
  Serial.print(F("[Bluetooth] Mac Address = "));
  Serial.print(mac5[0]);
  Serial.print(F(":"));
  Serial.print(mac5[1]);
  Serial.print(F(":"));
  Serial.print(mac5[2]);
  Serial.print(F(":"));
  Serial.print(mac5[3]);
  Serial.print(F(":"));
  Serial.print(mac5[4]);
  Serial.print(F(":"));
  Serial.print(mac5[5]);
  Serial.print(F("\r\n"));

  //ESPNOW
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.println("ESPNow/Basic/Master Example");
  Serial.print("STA MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("STA CHANNEL ");
  Serial.println(WiFi.channel());
  InitESPNow();
  esp_now_register_send_cb(OnDataSent);
}


void loop() {
  switch (g_mode) {

    case INIT_MODE:
      Serial.println("INIT_MODE");

      lock.openServo();
      ledRed.ledOff();
      ledGreen.ledOn();
      setMode(OPEN_MODE);
      break;

    case OPEN_MODE:
      Serial.println("OPEN_MODE");

      if (btn.buttonRead()) {
        Serial.println("BUTTON_PUSHED");
        setMode(OPEN_TO_LOCK_MODE);
        delay(50);
        break;
      }

      switch (g_action) {
        case SENSOR_ACTION:
          Serial.println("SENSOR_ACTION");

          if (sensor.checkIsInside()) {
            setAction(LOCK_ACTION);
            delay(5000);
          }
          delay(50);
          break;

        case LOCK_ACTION:
          Serial.println("LOCK_ACTION");

          setMode(OPEN_TO_LOCK_MODE);
          break;
      }
      break;

    case OPEN_TO_LOCK_MODE:
      Serial.println("OPEN_TO_LOCK_MODE");

      lock.lockServo();
      delay(1000);
      ledRed.ledOn();
      ledGreen.ledOff();
      setAction(SENSOR_ACTION);
      setMode(LOCK_MODE);

      ScanForSlave();
      if (slave.channel == CHANNEL) {  // check if slave channel is defined
        bool isPaired = manageSlave();
        if (isPaired) {
          sendData(BUZZER_SENDDATA);
        } else {
          Serial.println("Slave pair failed!");
        }
      }

      /*
      ScanForSlave();
      if (slave.channel == CHANNEL) {
        bool isPaired = manageSlave();
        if (isPaired) {
          sendData(LOCK_SENDDATA);
        } else {
          Serial.println("Slave pair failed!");
        }
      }
      */
      break;

    case LOCK_MODE:
      Serial.println("LOCK_MODE");

      if (!(ICRead()) && !(BTRead())) {
        break;
      } else {
        setMode(LOCK_TO_OPEN_MODE);
        break;
      }

    case LOCK_TO_OPEN_MODE:
      Serial.println("LOCK_TO_OPEN_MODE");
      
      lock.openServo();
      delay(1000);
      ledRed.ledOff();
      ledGreen.ledOn();
      setMode(OPEN_MODE);
      
      ScanForSlave();
      if (slave.channel == CHANNEL) {  // check if slave channel is defined
        bool isPaired = manageSlave();
        if (isPaired) {
          sendData(OPEN_SENDDATA);
        } else {
          Serial.println("Slave pair failed!");
        }
      }
      break;
  }
}


//関数***********************************************************************************
void setMode(MODE mode) {
  g_mode = mode;
}

MODE getMode() {
  return g_mode;
}

void setAction(ACTION act) {
  g_action = act;
}

ACTION getAction() {
  return g_action;
}

boolean ICRead() {
  boolean isReadRFID = false;

  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println(F("カードがありません"));
    delay(500);

  } else {
    Serial.println(F("読み込みました"));
    String hexString = convertHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(hexString);
    isReadRFID = true;
    delay(1000);
  }
  Serial.print("isReadRFID : ");
  Serial.println(isReadRFID);
  return isReadRFID;
}


boolean BTRead() {
  if (SerialBT.available()) {
    String buf = SerialBT.readStringUntil('\0');

    Serial.print("buf:");
    Serial.println(buf);

    if (buf == "open") {
      return true;
    }
  }
  return false;
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

/*
  void onTimerSensor() {
  timerSensor.isrCounter++;
  //右辺に動かしたい秒数を入れる
  if (timerSensor.isrCounter == secondSensor) {

    timerEnd(timerSensor.timer);
    timerSensor.timer = NULL;
    timerSensor.isrCounter = 0;

    //割り込み処理
    if (sensor.checkIsInside()) {
      setAction(LOCK_ACTION);
    }
    delay(1000);
  }
  }


  void onTimerLock() {
  timerLock.isrCounter++;
  //右辺に動かしたい秒数を入れる
  if (timerLock.isrCounter == secondLock) {

    timerEnd(timerLock.timer);
    timerLock.timer = NULL;
    timerLock.isrCounter = 0;

    //割り込み処理
    lock.lockServo();
    delay(1000);
  }
  }
*/

// Init ESP Now with fallback
void InitESPNow() {  //esp-nowの初期化処理
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

// Scan for slaves in AP mode
void ScanForSlave() {                                                          //子機をスキャンする
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL);  // Scan only on one channel
  // reset on each scan
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found ");
    Serial.print(scanResults);
    Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // SSIDとRSSIを表示
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
      }
      delay(10);
      // スキャンしたデバイスがslaveか確認
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        Serial.println("Found a Slave.");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" [");
        Serial.print(BSSIDstr);
        Serial.print("]");
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];
        if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
          for (int ii = 0; ii < 6; ++ii) {
            slave.peer_addr[ii] = (uint8_t)mac[ii];
          }
        }

        slave.channel = CHANNEL;  // pick a channel
        slave.encrypt = 0;        // no encryption　　　　暗号化処理・・・する必要ある？

        slaveFound = 1;
        // we are planning to have only one slave in this example;
        // Hence, break after we find one, to be a bit efficient
        break;
      }
    }
  }

  if (slaveFound) {
    Serial.println("Slave Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }

  // clean up ram
  WiFi.scanDelete();
}

// Check if the slave is already paired with the master.　　　　子機とのペアリング
// If not, pair the slave with master
bool manageSlave() {
  if (slave.channel == CHANNEL) {
    if (DELETEBEFOREPAIR) {
      deletePeer();
    }

    Serial.print("Slave Status: ");
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if (exists) {
      // Slave already paired.
      Serial.println("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(&slave);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}

void deletePeer() {  //子機のペアリング削除
  esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    // Delete success
    Serial.println("Success");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

//uint8_t data = 0;  //unit8_t　この関数を子機に送信する。
// send data
void sendData(uint8_t data) {  //送信する関数
  const uint8_t *peer_addr = slave.peer_addr;
  Serial.print("Sending: ");
  Serial.println(data);
  esp_err_t result = esp_now_send(peer_addr, &data, sizeof(data));
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

// callback when data is sent from Master to Slave　　
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
