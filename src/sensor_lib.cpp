//header files
#include "sensors.h"
#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <iostream>
#include <fstream>
#include "ADS131M04.h"
#include "daq_pins.h"

//class
SPIClass vspi(VSPI);
SPIClass sdspi(HSPI);

Adafruit_MAX31855 thermocouple1(TC_CS1, &vspi);
Adafruit_MAX31855 thermocouple2(TC_CS2, &vspi);
Adafruit_MAX31855 thermocouple3(TC_CS3, &vspi);
Adafruit_MAX31855 thermocouple4(TC_CS4, &vspi);
ADS131M04 ADC=ADS131M04(ADC_CS, CLKOUT, &vspi);

Sensors::Sensors(struct sensors){
    // struct containing data from sensors
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
        int32_t load1;
        int32_t load2;
        int32_t load3;
        int32_t load4;
        uint32_t currTime;
        bool EMatchBlown = false;
    };
    sensorData;
}

//create a file
File file;

//read ambient temp
double Sensors::readThermocoupleInternal(){
    return thermocouple1.readInternal();
}

//read temp
double Sensors::readThermocoupleCelsius(uint8_t i){
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

//read pressure
double Sensors::readPtap(uint16_t i){
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

void Sensors::dataLoop(bool EMatchState) {

    Serial.println("Logging data");

    //writing data into struct
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
    sensorData.currTime = millis();
    sensorData.EMatchBlown = EMatchState;

    // Read ADC data
    int32_t AdcOutArr[4];
    int8_t AdcChanArr[4] = {0, 1, 2, 3};

    ADC.rawChannels(&AdcChanArr[0], 4, &AdcOutArr[0]);

    sensorData.load1 = AdcOutArr[0];
    sensorData.load2 = AdcOutArr[1];
    sensorData.load3 = AdcOutArr[2];
    sensorData.load4 = AdcOutArr[3];
    
    // if(!file.print("yeet"))
    // {
    //    Serial.println("File's fucked mate");
    //}

    //printing data into csv file
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
    file.print(sensorData.currTime);
    file.print(",");
    file.print(sensorData.EMatchBlown);
    file.print(",");
    file.print("\n");

    file.flush();
}