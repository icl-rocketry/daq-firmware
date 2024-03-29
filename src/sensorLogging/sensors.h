#include <Arduino.h>
#include <Adafruit_MAX31855.h>
#include "ADS131M04.h"
#include "daq_pins.h"
#include <SD.h>

#ifndef SENSORS_H
#define SENSORS_H

void setupSensors();
double readThermocoupleInternal();
double readThermocoupleCelsius(uint8_t i);
struct sensors;
double readPtap(uint16_t i);
void readDispData(double* outputArrPtr); //KIRYL
void dataLoop(bool writeToSD);
bool setupSD();


#endif
