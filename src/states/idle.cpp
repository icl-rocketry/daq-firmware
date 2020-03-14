#include "Arduino.h"
#include "idle.h"
#include "state.h"
#include "WiFiButton/WiFiButton.h"
#include "sensorLogging/sensors.h"
#include "logging.h"

void Idle::initialise() {
}

State* Idle::update() {

  int wifiOutput = WIFIloop(this -> stateID);
  if (wifiOutput != 0) {
    Serial.println(wifiOutput);
  }
  /*
     CODE REMOVED FOR DEBUG PURPOSES ONLY, DO NOT MERGE
  // Print data to Serial for live plotting
  dataLoop(false);

  if (WIFIloop(this -> stateID)) {
    // If the button was pressed, return new instance of logging state
    State* logStatePtr = new Logging();
    return logStatePtr;

  } else {
    return this;
  }
  */
}
