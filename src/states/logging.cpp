#include "Arduino.h"
#include "logging.h"
#include "state.h"
#include "sensorLogging/sensors.h"
#include "daq_pins.h"

void Logging::initialise() {
 while (!setupSD(this)) {}
  digitalWrite(PYRO_CHANNEL_PIN, HIGH);
}

State* Logging::update() {
  dataLoop(true);
  return this;
}
