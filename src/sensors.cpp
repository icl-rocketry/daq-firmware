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
};

//create instance for struct
sensors sensorData;

//create a file
File file;

// set up the thermocouples
void setupSensors() {
    vspi.setClockDivider(SPI_CLOCK_DIV8);
    vspi.begin();

    ADC.begin();
    thermocouple1.begin();
    thermocouple2.begin();
    thermocouple3.begin();
    thermocouple4.begin();
    //initialise output variables as output
    pinMode(TC_CS1, OUTPUT);
    pinMode(TC_CS2, OUTPUT);
    pinMode(TC_CS3, OUTPUT);
    pinMode(TC_CS4, OUTPUT);
    pinMode(ADC_CS, OUTPUT);
    //initialise outputs as high
    digitalWrite(TC_CS1, HIGH);
    digitalWrite(TC_CS2, HIGH);
    digitalWrite(TC_CS3, HIGH);
    digitalWrite(TC_CS4, HIGH);
    digitalWrite(ADC_CS, HIGH);

}

//set up the SD card
void setupSD(){
    //spi pins
    sdspi.begin(14, 2, 15, -1);

    Serial.print("Initializing SD card...");
    pinMode(13, OUTPUT);

    //return error message if SD card not present
    if (!SD.begin(13, sdspi)) {
        Serial.println("Card failed, or not present");
        return;
    }
    Serial.println("Card initialized.");

    //opening file in SD 
    file = SD.open("/sensor_data.csv", FILE_WRITE);

    //error message if file is not opened
    if(!file)
    {
        Serial.println("File could not be opened");
    }

}

//read ambient temp
double readThermocoupleInternal(){
    return thermocouple1.readInternal();
}

//read temp
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

//read pressure
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
    file.print("\n");

    file.flush();
}


