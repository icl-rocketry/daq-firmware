#include <Arduino.h>
#include <Adafruit_MAX31855.h>
#include "ADS131M04.h"

#ifndef SENSORS_H
#define SENSORS_H

class Sensors {
    public:
        Sensors(uint8_t TC_CS1_PIN, uint8_t TC_CS2_PIN, uint8_t TC_TS3_PIN, uint8_t TC_CS4_PIN, uint8_t ADC_CS_PIN, uint8_t CLKOUT_PIN, uint8_t PTAP1_PIN, uint8_t PTAP2_PIN, uint8_t PTAP3_PIN, uint8_t PTAP4_PIN, uint8_t PTAP5_PIN);
        void setupSensors();
        double readThermocoupleInternal();
        double readThermocoupleCelsius(uint8_t i);
        double readPtap(uint16_t i);
        void dataLoop(bool EMatchState);
        bool setupSD();
    private:
        bool EMatchState;
        struct sensors;
        uint8_t _TC_CS1_PIN, _TC_CS2_PIN, _TC_CS3_PIN, _TC_CS4_PIN, _ADC_CS_PIN, _CLKOUT_PIN, _PTAP1_PIN, _PTAP2_PIN, _PTAP3_PIN, _PTAP4_PIN, _PTAP5_PIN;

  //class
  SPIClass vspi = SPIClass(VSPI);
  SPIClass sdspi = SPIClass(HSPI);

  Adafruit_MAX31855 thermocouple1 = Adafruit_MAX31855(_TC_CS1_PIN, &vspi);
  Adafruit_MAX31855 thermocouple2 = Adafruit_MAX31855(_TC_CS2_PIN, &vspi);
  Adafruit_MAX31855 thermocouple3 = Adafruit_MAX31855(_TC_CS3_PIN, &vspi);
  Adafruit_MAX31855 thermocouple4 = Adafruit_MAX31855(_TC_CS4_PIN, &vspi);
  ADS131M04 ADC = ADS131M04(_ADC_CS_PIN, _CLKOUT_PIN, &vspi);
};

#endif
