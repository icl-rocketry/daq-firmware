#include <Arduino.h>
#include "sensors.h"
#include "WiFiButton.h"
#include "daq_pins.h"

Sensors sensInst = Sensors(TC_CS1, TC_CS2, TC_CS3, TC_CS4, ADC_CS, CLKOUT, PTAP1, PTAP2, PTAP3, PTAP4, PTAP5);

void setup() {
}

void loop() {
  sensInst.dataLoop(true);
}
