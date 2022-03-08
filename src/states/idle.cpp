#include "Arduino.h"
#include "idle.h"
#include "state.h"
#include "WiFiButton/WiFiInterface.h"
#include "sensorLogging/sensors.h"
#include "logging.h"

void Idle::initialise() {

}

State* Idle::update() {

  dataLoop(false);

  if (WIFIloop(false)) {
    // If the button was pressed, return new instance of logging state
    State* logStatePtr = new Logging();
    return logStatePtr;

  } else {
    return this;
  }
}
