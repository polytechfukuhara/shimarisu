// Bluetooth通信体験プログラム
// ESP32用のプログラム
// Arduinoとシリアル通信を、タブレットとBluetooth通信を行います

#include <dummy.h>
#include <uTimerLib.h>
#include <BluetoothSerial.h>

//インスタンス化
BluetoothSerial SerialBT;


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_SHIMARISU"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  uint8_t mac5[6];
  esp_read_mac(mac5, ESP_MAC_BT);
  Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac5[0], mac5[1], mac5[2], mac5[3], mac5[4], mac5[5]);
}

void loop() {

  while (SerialBT.available()) {
    String buf = SerialBT.readStringUntil('\0');

    Serial.print("buf:");
    Serial.println(buf);

    /*
        if(buf.substring(0,3).equals("LED")){
          int interval_tmp = buf.substring(3).toInt();
          if(0 < interval_tmp && interval_tmp < 4){
            led_interval = interval_tmp;
            TimerLib.setInterval_s(led_blink, led_interval);
            SerialBT.print("LED blinks for " + String(led_interval) + " sec");
          }
        }else if(buf.substring(0,3).equals("OFF")){
          TimerLib.clearTimer();
          SerialBT.print("OFF");
          digitalWrite(LED_PIN, LOW);
        }
    */

  }

}
