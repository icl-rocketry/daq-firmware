// class Sensors {
//     public:

// };
#include <Arduino.h>
#include <Adafruit_MAX31855.h>
#include "daq_pins.h"

void setupSensors();
double readThermocoupleInternal();
double readThermocoupleCelsius(uint8_t i);
double readPtap(uint16_t i);
