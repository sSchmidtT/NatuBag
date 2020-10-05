#include <Arduino.h>

const uint8_t LED_SUCCESS = D2;
const uint8_t LED_ALERT = D3;
const uint8_t LED_DANGER = D4;
const uint8_t LED_ON = D8;

int ledState = 0;

// 0 - initial
// 1 - success
// 2 - alert
// 3 - danger
int blink = 80;

void initializeLeds(){
    Serial.println("Inicializando leds");
    pinMode(LED_ON, OUTPUT);
    digitalWrite(LED_ON, LOW);
    pinMode(LED_SUCCESS, OUTPUT);
    digitalWrite(LED_SUCCESS, LOW);
    pinMode(LED_ALERT, OUTPUT);
    digitalWrite(LED_ALERT, LOW);
    pinMode(LED_DANGER, OUTPUT);
    digitalWrite(LED_DANGER, LOW);
}

void onError(){
    digitalWrite(LED_SUCCESS, LOW);
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(LED_ON, LOW);
    digitalWrite(LED_DANGER, HIGH);
}

void onInitial(){
    digitalWrite(LED_SUCCESS, LOW);
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(LED_DANGER, LOW);
    digitalWrite(LED_ON, HIGH);
}

void onSuccess(){
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(LED_ON, LOW);
    digitalWrite(LED_DANGER, LOW);
    digitalWrite(LED_SUCCESS, HIGH);

}

void onAlert(){
    digitalWrite(LED_SUCCESS, LOW);
    digitalWrite(LED_ON, LOW);
    digitalWrite(LED_DANGER, LOW);
    digitalWrite(LED_ALERT, HIGH);
}

void blinkLedInitial(int xVezes){
    delay(blink);
    for(int i = 0; i < xVezes; i++){
        digitalWrite(LED_ON, LOW);
        delay(blink);
        digitalWrite(LED_ON, HIGH);
        delay(blink);
    }
    
}
void blinkLedDanger(int xVezes){
    delay(blink);
    for(int i = 0; i < xVezes; i++){
        digitalWrite(LED_DANGER, LOW);
        delay(blink);
        digitalWrite(LED_DANGER, HIGH);
        delay(blink);
    }
}
void blinkLedAlert(int xVezes){
    delay(blink);
    for(int i = 0; i < xVezes; i++){
        digitalWrite(LED_ALERT, LOW);
        delay(blink);
        digitalWrite(LED_ALERT, HIGH);
        delay(blink);
    }
}
void blinkLedSuccess(int xVezes){
    delay(blink);
    for(int i = 0; i < xVezes; i++){
        digitalWrite(LED_SUCCESS, LOW);
        delay(blink);
        digitalWrite(LED_SUCCESS, HIGH);
        delay(blink);
    } 
}