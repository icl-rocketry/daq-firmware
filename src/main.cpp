#include <Arduino.h>
#include "sensors.h"
#include "pyro.h"

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  
  setupSensors();
  setupSD();
  setupWIFI();
}

void loop() {
  dataLoop(WIFIloop());
  delay(500);
}
