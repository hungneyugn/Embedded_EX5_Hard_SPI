#include <SPI.h>

#define SS_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13
uint8_t SS_State;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(SS_PIN, INPUT_PULLUP);
}

void loop() {
  SS_State = digitalRead(SS_PIN);
  if (SS_State == LOW) {
    uint8_t receivedData = SPI.transfer(0x00); 
    Serial.print("Received data from STM32F1: ");
    Serial.println(receivedData, BIN);
    while(SS_State == LOW) SS_State = digitalRead(SS_PIN);
  }
}
