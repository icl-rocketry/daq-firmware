#include <Arduino.h>
#include <Adafruit_MAX31855.h>
#include "ADS131M04.h"
#include "daq_pins.h"

#ifndef SENSORS_H
#define SENSORS_H

void setupSensors();
double readThermocoupleInternal();
double readThermocoupleCelsius(uint8_t i);
double readPtap(uint16_t i);
void dataLoop(bool EMatchState);
bool setupSD();
bool EMatchState;
struct sensors;

// Class initialisations

File file;

SPIClass vspi = SPIClass(VSPI);
SPIClass sdspi = SPIClass(HSPI);

Adafruit_MAX31855 thermocouple1 = Adafruit_MAX31855(TC_CS1, &vspi);
Adafruit_MAX31855 thermocouple2 = Adafruit_MAX31855(TC_CS2, &vspi);
Adafruit_MAX31855 thermocouple3 = Adafruit_MAX31855(TC_CS3, &vspi);
Adafruit_MAX31855 thermocouple4 = Adafruit_MAX31855(TC_CS4, &vspi);
ADS131M04 ADC = ADS131M04(ADC_CS, CLKOUT, &vspi);

#endif
