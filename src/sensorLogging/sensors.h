#include <Arduino.h>
#include <Adafruit_MAX31855.h>
#include "ADS131M04.h"
#include "daq_pins.h"
#include <SD.h>
#include "states/state.h"
#include "states/logging.h"
#include "states/calibration.h"

#ifndef SENSORS_H
#define SENSORS_H

void setupSensors();
double readThermocoupleInternal();
double readThermocoupleCelsius(uint8_t i);
double readPtap(uint16_t i);
void dataLoop(bool writeToSD);
bool setupSD(State* logStatePtr);
struct sensors;


#endif
