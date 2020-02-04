#include <Arduino.h>
#include <Adafruit_MAX31855.h>
#include "daq_pins.h"

class Sensors{
    public:
        Sensors(struct sensors);
        void setupSensors();
        double readThermocoupleInternal();
        double readThermocoupleCelsius(uint8_t i);
        double readPtap(uint16_t i);
        void dataLoop(bool EMatchState);
        bool setupSD();
    private:
        bool EMatchState;
        struct sensors;
};

