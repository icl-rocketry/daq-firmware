//header files
#include "sensors.h"
#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <iostream>
#include <fstream>
#include "ADS131M04.h"
#include <Adafruit_MAX31855.h>
#include "daq_pins.h"

bool EMatchState;

// Class initialisations

File file;

SPIClass vspi = SPIClass(VSPI);
SPIClass sdspi = SPIClass(HSPI);

Adafruit_MAX31855 thermocouple1 = Adafruit_MAX31855(TC_CS1, &vspi);
Adafruit_MAX31855 thermocouple2 = Adafruit_MAX31855(TC_CS2, &vspi);
Adafruit_MAX31855 thermocouple3 = Adafruit_MAX31855(TC_CS3, &vspi);
Adafruit_MAX31855 thermocouple4 = Adafruit_MAX31855(TC_CS4, &vspi);
ADS131M04 ADC = ADS131M04(ADC_CS, CLKOUT, &vspi);

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

    // Set ADC gain to 4
    ADC.setGain(2,2,2,2);

    // Activate global chop mode on the ADC
    ADC.globalChop(true);
}

//set up the SD card
bool setupSD(){
    //spi pins
    sdspi.begin(14, 2, 15, -1);

    Serial.print("Initializing SD card...");
    pinMode(13, OUTPUT);

    //return error message if SD card not present
    if (!SD.begin(13, sdspi)) {
        Serial.println("Card failed, or not present");
        return false;
    }
    Serial.println("Card initialized.");

    //opening file in SD 
    file = SD.open("/sensor_data.csv", FILE_APPEND);

    //error message if file is not opened
    if(!file)
    {
        Serial.println("File could not be opened");
        return false;
    }
    return true;
}

//read ambient temp
double readThermocoupleInternal(){
    return thermocouple1.readInternal();
}

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
// KIRYL
void readDispData(double* outputArrPtr){
    //writing data into struct
    *outputArrPtr = thermocouple1.readInternal();  // Ambient temp 
    outputArrPtr++; 
    *outputArrPtr = thermocouple1.readCelsius();
    outputArrPtr++; 
    *outputArrPtr = thermocouple2.readCelsius();
    outputArrPtr++; 
    *outputArrPtr = thermocouple3.readCelsius();
    outputArrPtr++; 
    *outputArrPtr = thermocouple4.readCelsius();
    outputArrPtr++; 
    *outputArrPtr = analogRead(PTAP1);
    outputArrPtr++; 
    *outputArrPtr = analogRead(PTAP2);
    outputArrPtr++; 
    *outputArrPtr = analogRead(PTAP3);
    outputArrPtr++; 
    *outputArrPtr = analogRead(PTAP4);
    outputArrPtr++; 
    *outputArrPtr = analogRead(PTAP5);
    outputArrPtr++; 
    *outputArrPtr = millis();
    outputArrPtr++; 

    // Read ADC data
    int32_t AdcOutArr[4];
    int8_t AdcChanArr[4] = {0, 1, 2, 3};

    ADC.rawChannels(&AdcChanArr[0], 4, &AdcOutArr[0]);

    *outputArrPtr = AdcOutArr[0];
    outputArrPtr++; 
    *outputArrPtr = AdcOutArr[1];
    outputArrPtr++; 
    *outputArrPtr = AdcOutArr[2];
    outputArrPtr++; 
    *outputArrPtr = AdcOutArr[3];
}

void dataLoop(bool writeToSD) {

    Serial.println("Logging data");
    sensors _sensorData;

    //writing data into struct
    _sensorData.thermoAmbient = thermocouple1.readInternal();    
    _sensorData.thermo1 = thermocouple1.readCelsius();
    _sensorData.thermo2 = thermocouple2.readCelsius();
    _sensorData.thermo3 = thermocouple3.readCelsius();
    _sensorData.thermo4 = thermocouple4.readCelsius();
    _sensorData.ptap1 = analogRead(PTAP1);
    _sensorData.ptap2 = analogRead(PTAP2);
    _sensorData.ptap3 = analogRead(PTAP3);
    _sensorData.ptap4 = analogRead(PTAP4);
    _sensorData.ptap5 = analogRead(PTAP5);
    _sensorData.currTime = millis();

    // Read ADC data
    int32_t AdcOutArr[4];
    int8_t AdcChanArr[4] = {0, 1, 2, 3};

    ADC.rawChannels(&AdcChanArr[0], 4, &AdcOutArr[0]);

    _sensorData.load1 = AdcOutArr[0];
    _sensorData.load2 = AdcOutArr[1];
    _sensorData.load3 = AdcOutArr[2];
    _sensorData.load4 = AdcOutArr[3];
    
    // if(!outputPtr->print("yeet"))
    // {
    //    Serial.println("File's fucked mate");
    //}

    Stream* outputPtr;
    // Check where to write data
    if (writeToSD == true) {
      outputPtr = &file;
    } else {
      outputPtr = &Serial;
    }

    //printing data into csv file
    outputPtr->print(_sensorData.thermoAmbient);
    outputPtr->print(",");
    outputPtr->print(_sensorData.thermo1);
    outputPtr->print(",");
    outputPtr->print(_sensorData.thermo2);
    outputPtr->print(",");
    outputPtr->print(_sensorData.thermo3);
    outputPtr->print(",");
    outputPtr->print(_sensorData.thermo4);
    outputPtr->print(",");
    outputPtr->print(_sensorData.ptap1);
    outputPtr->print(",");
    outputPtr->print(_sensorData.ptap2);
    outputPtr->print(",");
    outputPtr->print(_sensorData.ptap3);
    outputPtr->print(",");
    outputPtr->print(_sensorData.ptap4);
    outputPtr->print(",");
    outputPtr->print(_sensorData.ptap5);
    outputPtr->print(",");
    outputPtr->print(_sensorData.load1);
    outputPtr->print(",");
    outputPtr->print(_sensorData.load2);
    outputPtr->print(",");
    outputPtr->print(_sensorData.load3);
    outputPtr->print(",");
    outputPtr->print(_sensorData.load4);
    outputPtr->print(",");
    outputPtr->print(_sensorData.currTime);
    outputPtr->print("\n");

    outputPtr->flush();
}
