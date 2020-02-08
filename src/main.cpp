#include <Arduino.h>
#include "sensors.h"
#include "pyro.h"

Sensors sensInst = Sensors();

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  
  sensInst.setupSensors();

  // Do nothing until the SD card has initialised
  while (!sensInst.setupSD()) {}
  setupWIFI();

  bool EMatchBlown=false;
  while (!EMatchBlown) {
    EMatchBlown = WIFIloop();
  }
}

void loop() {
  sensInst.dataLoop(true);
}
