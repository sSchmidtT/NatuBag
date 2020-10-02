#include <secrets.h>

#include <wifi.h>
#include <ntp.h>
#include <mqtt.h>
#include <rfid.h>

String tag;

void checkWiFiThenMQTT(void)
{
  connectToWiFi("Checking WiFi");
  connectToMqtt();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println();

  setupWifi();
  NTPConnect();
  setupSSL();
  setupMQTT();
  setupRFid();

  connectToMqtt();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(!client.connected()){
    checkWiFiThenMQTT();
  }else{
    if(! rfid.PICC_IsNewCardPresent())
    return;
  
    if(rfid.PICC_ReadCardSerial()){
      Serial.println("Lendo Cartão!");
      for(byte i = 0; i < rfid.uid.size; i++){
          tag += rfid.uid.uidByte[i];
      }
      Serial.print("RFid read: ");
      Serial.println(tag);
      sendDataAddProduct(tag);
      tag = "";
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
    client.loop();
  }  
}