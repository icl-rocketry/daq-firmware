#include <Arduino.h>
#include "sensors.h"
#include "pyro.h"

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  
  setupSensors();

  // Do nothing until the SD card has initialised
  while (!setupSD()) {}
  setupWIFI();

  bool EMatchBlown=false;
  while (!EMatchBlown) {
    EMatchBlown = WIFIloop();
  }
}

void loop() {
  dataLoop(true);
}
