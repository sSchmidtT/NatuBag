#include <secrets.h>
#include <global.h>

#include <ntp.h>
#include <leds.h>
#include <wifi.h>
#include <mqtt.h>
#include <button.h>
#include <rfid.h>


void checkWiFiThenMQTT(void)
{
  connectToWiFi("Checking WiFi");
  connectToMqtt();
  if(inQueue){
    onAlert();
    ledState = 3;
  }
  else{
    onInitial();
    ledState = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  initializeGlobalVar();
  initializeLeds();
  onError();
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println();
  blinkLedDanger(1);
  initButton();
  blinkLedDanger(1);
  setupRFid();
  blinkLedDanger(1);
  
  setupWifi();
  blinkLedDanger(1);
  NTPConnect();
  blinkLedDanger(1);
  setupSSL();
  blinkLedDanger(1);
  setupMQTT();
  blinkLedDanger(1);

  connectToMqtt();
  blinkLedDanger(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(inInital){
    sendDataInitialBag(true);
    inInital = false;
    onInitial();
  }
  if(!client.connected()){
    Serial.println("Connection MQTT not connected");
    onError();
    ledState = 3;
    checkWiFiThenMQTT();
  }else{
    client.loop();
    onClick();
    if(!inQueue && !inAttendance){
      readRFid();
    }
    
  }
  if(ledState == 0)
    blinkLedInitial(1);
  else if(ledState == 1)
    blinkLedSuccess(1);
  else if(ledState == 2)
    blinkLedAlert(1);
  else if(ledState == 3)
    blinkLedDanger(1);
  
}