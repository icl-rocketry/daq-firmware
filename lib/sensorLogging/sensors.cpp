//header files
#include "sensors.h"
#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <iostream>
#include <fstream>
#include "ADS131M04.h"
#include <Adafruit_MAX31855.h>


Sensors::Sensors(uint8_t TC_CS1_PIN, uint8_t TC_CS2_PIN, uint8_t TC_CS3_PIN, uint8_t TC_CS4_PIN, uint8_t ADC_CS_PIN, uint8_t CLKOUT_PIN, uint8_t PTAP1_PIN, uint8_t PTAP2_PIN, uint8_t PTAP3_PIN, uint8_t PTAP4_PIN, uint8_t PTAP5_PIN){
  _TC_CS1_PIN = TC_CS1_PIN;
  _TC_CS2_PIN = TC_CS2_PIN;
  _TC_CS3_PIN = TC_CS3_PIN;
  _TC_CS4_PIN = TC_CS4_PIN;
  _ADC_CS_PIN = ADC_CS_PIN;
  _CLKOUT_PIN = CLKOUT_PIN;
  _PTAP1_PIN = PTAP1_PIN;
  _PTAP2_PIN = PTAP2_PIN;
  _PTAP3_PIN = PTAP3_PIN;
  _PTAP4_PIN = PTAP4_PIN;
  _PTAP5_PIN = PTAP5_PIN;

}

void Sensors::setupSensors() {
    vspi.setClockDivider(SPI_CLOCK_DIV8);
    vspi.begin();

    ADC.begin();
    thermocouple1.begin();
    thermocouple2.begin();
    thermocouple3.begin();
    thermocouple4.begin();
    //initialise output variables as output
    pinMode(_TC_CS1_PIN, OUTPUT);
    pinMode(_TC_CS2_PIN, OUTPUT);
    pinMode(_TC_CS3_PIN, OUTPUT);
    pinMode(_TC_CS4_PIN, OUTPUT);
    pinMode(_ADC_CS_PIN, OUTPUT);
    //initialise outputs as high
    digitalWrite(_TC_CS1_PIN, HIGH);
    digitalWrite(_TC_CS2_PIN, HIGH);
    digitalWrite(_TC_CS3_PIN, HIGH);
    digitalWrite(_TC_CS4_PIN, HIGH);
    digitalWrite(_ADC_CS_PIN, HIGH);

    // Set ADC gain to 4
    ADC.setGain(2,2,2,2);

    // Activate global chop mode on the ADC
    ADC.globalChop(true);
}

//create a file
File file;

//set up the SD card
bool Sensors::setupSD(){
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
double Sensors::readThermocoupleInternal(){
    return thermocouple1.readInternal();
}

// struct containing data from sensors
struct Sensors::sensors {
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
        return analogRead(_PTAP1_PIN);
        break;

        case 2:
        return analogRead(_PTAP2_PIN);
        break;

        case 3:
        return analogRead(_PTAP3_PIN);
        break;

        case 4:
        return analogRead(_PTAP4_PIN);
        break;

        case 5:
        return analogRead(_PTAP5_PIN);
        break;

        default:
        return -1;
    }
}

void Sensors::dataLoop(bool EMatchState) {

    Serial.println("Logging data");
    sensors _sensorData;

    //writing data into struct
    _sensorData.thermoAmbient = thermocouple1.readInternal();    
    _sensorData.thermo1 = thermocouple1.readCelsius();
    _sensorData.thermo2 = thermocouple2.readCelsius();
    _sensorData.thermo3 = thermocouple3.readCelsius();
    _sensorData.thermo4 = thermocouple4.readCelsius();
    _sensorData.ptap1 = analogRead(_PTAP1_PIN);
    _sensorData.ptap2 = analogRead(_PTAP2_PIN);
    _sensorData.ptap3 = analogRead(_PTAP3_PIN);
    _sensorData.ptap4 = analogRead(_PTAP4_PIN);
    _sensorData.ptap5 = analogRead(_PTAP5_PIN);
    _sensorData.currTime = millis();
    _sensorData.EMatchBlown = EMatchState;

    // Read ADC data
    int32_t AdcOutArr[4];
    int8_t AdcChanArr[4] = {0, 1, 2, 3};

    ADC.rawChannels(&AdcChanArr[0], 4, &AdcOutArr[0]);

    _sensorData.load1 = AdcOutArr[0];
    _sensorData.load2 = AdcOutArr[1];
    _sensorData.load3 = AdcOutArr[2];
    _sensorData.load4 = AdcOutArr[3];
    
    // if(!file.print("yeet"))
    // {
    //    Serial.println("File's fucked mate");
    //}

    //printing data into csv file
    file.print(_sensorData.thermoAmbient);
    file.print(",");
    file.print(_sensorData.thermo1);
    file.print(",");
    file.print(_sensorData.thermo2);
    file.print(",");
    file.print(_sensorData.thermo3);
    file.print(",");
    file.print(_sensorData.thermo4);
    file.print(",");
    file.print(_sensorData.ptap1);
    file.print(",");
    file.print(_sensorData.ptap2);
    file.print(",");
    file.print(_sensorData.ptap3);
    file.print(",");
    file.print(_sensorData.ptap4);
    file.print(",");
    file.print(_sensorData.ptap5);
    file.print(",");
    file.print(_sensorData.load1);
    file.print(",");
    file.print(_sensorData.load2);
    file.print(",");
    file.print(_sensorData.load3);
    file.print(",");
    file.print(_sensorData.load4);
    file.print(",");
    file.print(_sensorData.currTime);
    file.print(",");
    file.print(_sensorData.EMatchBlown);
    file.print(",");
    file.print("\n");

    file.flush();
}
