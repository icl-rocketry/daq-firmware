#include "Arduino.h"
#include "logging.h"
#include "state.h"
#include "WiFiButton/WiFiButton.h"
#include "sensorLogging/sensors.h"
#include "daq_pins.h"


void Logging::initialise() {
  //digitalWrite(PYRO_CHANNEL_PIN, HIGH);
}

State* Logging::update() {
  WIFIloop(true);
  dataLoop(true);
  return this;
}
