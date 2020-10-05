#include <SPI.h>
#include <MFRC522.h>

const uint8_t RST_PIN = D0;
const uint8_t SS_PIN = D1;

String tag;
String tagVerify;
bool rfSelected = false;
int readInterval = 5000;
unsigned long lastRfidMillis = 0;

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key RC522_key;

void setupRFid(){
    Serial.println("Inicializando leitor de RFid");
    SPI.begin();
    rfid.PCD_Init();
}

void cleanSelectedInative(){
    if ((millis() - lastRfidMillis > readInterval) && rfSelected){
        Serial.println("Limpando seleção!");
        lastRfidMillis = millis();
        tag = "";
        tagVerify = "";
        rfSelected = false;
        onError();
        blinkLedDanger(8);
        onInitial();
        ledState = 0;
    }
}

void readRFid()
{
    now = time(nullptr);
    cleanSelectedInative();
    if(! rfid.PICC_IsNewCardPresent())
        return;
    
    if(rfid.PICC_ReadCardSerial()){
        onAlert();
        ledState = 2;
        Serial.println("Lendo Cartão!");
        for(byte i = 0; i < rfid.uid.size; i++){
            tag += rfid.uid.uidByte[i];
        }
        Serial.print("RFid read: ");
        Serial.println(tag);
        if(!rfSelected){
            lastRfidMillis = millis();
            rfSelected = true;
            tagVerify = tag;
            tag = "";
        }else
        {   Serial.println("Verificando RFid");
            if(tagVerify == tag){
                Serial.println("RFid Verificado, add produto!");
                sendDataAddProduct(tag);
                tag = "";
                tagVerify = "";
                rfSelected = false;
                onSuccess();
                blinkLedSuccess(8);
                onInitial();
                ledState = 0;
            }else
            { 
                tag = "";
                tagVerify = "";
                rfSelected = false;
                onError();
                blinkLedDanger(8);
                onInitial();
                ledState = 0;
            }
        }
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
    }
}




