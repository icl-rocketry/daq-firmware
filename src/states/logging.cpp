#include "Arduino.h"
#include "logging.h"
#include "state.h"
#include "sensorLogging/sensors.h"
#include "daq_pins.h"
#include "WiFiButton.h"

void Logging::initialise() {
  digitalWrite(PYRO_CHANNEL_PIN, HIGH);
}

State* Logging::update() {
  if(pullDispSett()){
    dataLoop(true);
  }
  return this;
}
