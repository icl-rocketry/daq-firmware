// Load Wi-Fi library
#include <WiFi.h>
#include "sensors.h"

#ifndef WIFIBUTTON_H
#define WIFIBUTTON_H

void setupWIFI();
bool WIFIloop();

// WIFI credentials
const char *ssid = "ICLR_DAQ";
const char *password = "rocketsAreCool!";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String pyroState = "off";

#endif
