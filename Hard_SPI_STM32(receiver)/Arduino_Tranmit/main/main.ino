#include <SPI.h>

#define SS_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13

void setup() {
  SPI.begin();
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, LOW);
}

void loop() {
  digitalWrite(SS_PIN, LOW);
  uint8_t dataToSend = 0xAB;
  SPI.transfer(dataToSend);
  digitalWrite(SS_PIN, HIGH);
  delay(1000);
}

