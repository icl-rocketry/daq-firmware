/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h" 
#include "stateMachine.h"
#include "states/state.h"

bool stateMachine::initialise(State* initStatePtr) {
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  
  sensInst.setupSensors();

  // Do nothing until the SD card has initialised
  while (!sensInst.setupSD()) {}
  setupWIFI();

  bool EMatchBlown=false;
  while (!EMatchBlown) {
    EMatchBlown = WIFIloop();
  }
  changeState(initStatePtr);
}

void stateMachine::update() {
  State* newStatePtr = _currStatePtr -> update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }
}

void stateMachine::changeState(State* newStatePtr) {
  // Delete old state instance and change to new one
  delete _currStatePtr;
  _currStatePtr = newStatePtr;
  _currStatePtr -> initialise();
}

