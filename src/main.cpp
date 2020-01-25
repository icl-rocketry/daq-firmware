#include <Arduino.h>
#include "sensors.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  
  setupSensors();
  setupSD();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Serial.print("Internal Temperature = ");
  // Serial.println(readThermocoupleInternal());
  // Serial.print("c = ");
  // Serial.println(readThermocoupleCelsius(1));
  // Serial.print("Pressure = ");
  // Serial.println(readPtap(1));
  delay(500);
  dataLoop();
}