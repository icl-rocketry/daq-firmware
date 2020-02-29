#include "Arduino.h"
#include "calibration.h"
#include "state.h"
#include "WiFiButton/WiFiButton.h"
#include "sensorLogging/sensors.h"

void Calibration::initialise() {

}

State* Calibration::update() {
  dataLoop(true);

  if (WIFIloop()) {
    // If the button was pressed, return new instance of logging state
    State* logStatePtr = new Logging();
    return logStatePtr;

  } else {
    return this;
  }
}
