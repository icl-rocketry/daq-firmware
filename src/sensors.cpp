#include "sensors.h"

SPIClass vspi(VSPI);

Adafruit_MAX31855 thermocouple1(TC_CS1, &vspi);
Adafruit_MAX31855 thermocouple2(TC_CS2, &vspi);
Adafruit_MAX31855 thermocouple3(TC_CS3, &vspi);
Adafruit_MAX31855 thermocouple4(TC_CS4, &vspi);

void setupSensors() {
    vspi.setClockDivider(SPI_CLOCK_DIV8);
    vspi.begin();

    thermocouple1.begin();
    thermocouple2.begin();
    thermocouple3.begin();
    thermocouple4.begin();
    pinMode(TC_CS1, OUTPUT);
    pinMode(TC_CS2, OUTPUT);
    pinMode(TC_CS3, OUTPUT);
    pinMode(TC_CS4, OUTPUT);
    pinMode(ADC_CS, OUTPUT);
    digitalWrite(TC_CS1, HIGH);
    digitalWrite(TC_CS2, HIGH);
    digitalWrite(TC_CS3, HIGH);
    digitalWrite(TC_CS4, HIGH);
    digitalWrite(ADC_CS, HIGH);

}
double readThermocoupleInternal(uint8_t i) {
    switch(i){
        case 1 :
        return thermocouple1.readInternal();
        break;
        
        case 2:
        return thermocouple2.readInternal();
        break;

        case 3:
        return thermocouple3.readInternal();
        break;

        case 4:
        return thermocouple4.readInternal();
        break;

        default:
        return -1;
    }
    
    
}
double readThermocoupleCelsius(uint8_t i){
    switch(i){
        case 1:
        return  thermocouple1.readCelsius();
        break;

        case 2:
        return thermocouple2.readCelsius();
        break;

        case 3:
        return thermocouple3.readCelsius();
        break;

        case 4:
        return thermocouple4.readCelsius();
        break;

        default:
        return -1;
    }
}

double readPtap(uint16_t i){
    switch(i){
        case 1:
        return analogRead(PTAP1);
        break;

        case 2:
        return analogRead(PTAP2);
        break;

        case 3:
        return analogRead(PTAP3);
        break;

        case 4:
        return analogRead(PTAP4);
        break;

        case 5:
        return analogRead(PTAP5);
        break;

        default:
        return -1;
    }
}