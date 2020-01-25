#include "sensors.h"
#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <iostream>
#include <fstream>

SPIClass vspi(VSPI);
SPIClass sdspi(HSPI);

Adafruit_MAX31855 thermocouple1(TC_CS1, &vspi);
Adafruit_MAX31855 thermocouple2(TC_CS2, &vspi);
Adafruit_MAX31855 thermocouple3(TC_CS3, &vspi);
Adafruit_MAX31855 thermocouple4(TC_CS4, &vspi);

struct sensors {
    double thermoAmbient;
    double thermo1;
    double thermo2;
    double thermo3;
    double thermo4;
    double ptap1;
    double ptap2;
    double ptap3;
    double ptap4;
    double ptap5;
    uint16_t load1;
    uint16_t load2;
    uint16_t load3;
    uint16_t load4;
};

sensors sensorData;

File file;

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

void setupSD(){
    sdspi.begin(14, 2, 15, -1);

    Serial.print("Initializing SD card...");
    pinMode(13, OUTPUT);

    if (!SD.begin(13, sdspi)) {
        Serial.println("Card failed, or not present");
        return;
    }
    Serial.println("Card initialized.");
    // fout.open("sensors_data.csv", std::ios::out | std::ios::app);

    file = SD.open("/sensor_data2.csv", FILE_WRITE);

    if(!file)
    {
        Serial.println("File could not be opened");
    }

}

double readThermocoupleInternal(){
    return thermocouple1.readInternal();
}

double readThermocoupleCelsius(uint8_t i){
    switch(i){
        case 1:
        return thermocouple1.readCelsius();
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

void dataLoop() {

    Serial.println("Logging data");

    sensorData.thermoAmbient = thermocouple1.readInternal();    
    sensorData.thermo1 = thermocouple1.readCelsius();
    sensorData.thermo2 = thermocouple2.readCelsius();
    sensorData.thermo3 = thermocouple3.readCelsius();
    sensorData.thermo4 = thermocouple4.readCelsius();
    sensorData.ptap1 = analogRead(PTAP1);
    sensorData.ptap2 = analogRead(PTAP2);
    sensorData.ptap3 = analogRead(PTAP3);
    sensorData.ptap4 = analogRead(PTAP4);
    sensorData.ptap5 = analogRead(PTAP5);
    
    if(!file.print("yeet"))
    {
        Serial.println("File's fucked mate");
    }

    file.print(sensorData.thermoAmbient);
    file.print(",");
    file.print(sensorData.thermo1);
    file.print(",");
    file.print(sensorData.thermo2);
    file.print(",");
    file.print(sensorData.thermo3);
    file.print(",");
    file.print(sensorData.thermo4);
    file.print(",");
    file.print(sensorData.ptap1);
    file.print(",");
    file.print(sensorData.ptap2);
    file.print(",");
    file.print(sensorData.ptap3);
    file.print(",");
    file.print(sensorData.ptap4);
    file.print(",");
    file.print(sensorData.ptap5);
    file.print(",");
    file.print(sensorData.load1);
    file.print(",");
    file.print(sensorData.load2);
    file.print(",");
    file.print(sensorData.load3);
    file.print(",");
    file.print(sensorData.load4);
    file.print(",");
    file.print("\n");

    file.flush();
}


