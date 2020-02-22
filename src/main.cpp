#include <Arduino.h>
#include "stateMachine.h"
#include "sensorLogging/sensors.h"
#include "WiFiButton/WiFiButton.h"
#include "states/idle.h"

stateMachine stateMach;
Idle initialState;

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(115200);

  // Initialise logging hardware
  while (!setupSD()) {}
  setupSensors();

  // Initialise the interface
  setupWIFI();

  // Start the state machine with the correct initial state
  stateMach.initialise(new Idle);
}

void loop() {
  // Rerun the state machine
  stateMach.update();
}
