#include <Arduino.h>

const uint8_t ENTRY_QUEUE = 9;

unsigned long lastButtonMillis;
bool buttonClicked = false;
unsigned long duration = 0;


void initButton(){
    Serial.println("Leitura do botão");
    pinMode(ENTRY_QUEUE, INPUT);
    lastButtonMillis = millis();
}

void onClick(){
    if(digitalRead(ENTRY_QUEUE) != 0){
        if(!buttonClicked){
            lastButtonMillis = millis();
            buttonClicked = true;
        }
    }else
    {
        if(buttonClicked){
            duration = (millis() - lastButtonMillis);
        }
    }

    if(duration > 0 && buttonClicked){
        Serial.print("Duração do click: ");
        Serial.println(duration);

        if(inInative && duration > 300){
            inInative = false;
            ledState = 0;
            inInital = true;
        }
        else if(!inQueue && duration > 300){
            Serial.println("Pedido para entrar em fila para caixa!!!");
            onAlert();
            inQueue = true;
            sendDataAddQueue(true);
            ledState = 2;
        }else if(inQueue && duration > 1000){
            Serial.println("Pedido para sair da fila para caixa!!!");
            onInitial();
            inQueue = false;
            sendDataAddQueue(false);
            ledState = 0;
        }

        duration = 0;
        buttonClicked = false;
    }
    
}