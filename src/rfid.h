#include <SPI.h>
#include <MFRC522.h>

const uint8_t RST_PIN = D3;
const uint8_t SS_PIN = D4;

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key RC522_key;

void setupRFid(){
    Serial.println("Inicializando leitor de RFid");
    SPI.begin();
    rfid.PCD_Init();
}




