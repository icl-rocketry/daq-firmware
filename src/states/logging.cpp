#include "Arduino.h"
#include "logging.h"
#include "state.h"
#include "WiFiButton/WiFiInterface.h"
#include "sensorLogging/sensors.h"
#include "daq_pins.h"
#include "../WiFiButton/WiFiInterface.h"
#include "states/idle.h"


void Logging::initialise() {
  //digitalWrite(PYRO_CHANNEL_PIN, HIGH);
}

State* Logging::update() {
  dataLoop(true);

  if (!WIFIloop(true)) {
    // If the interface says that we should not be logging, switch to idle state
    State* idleStatePtr = new Idle();
    return idleStatePtr;

  } else {
    return this;
  }
  return this;
}
