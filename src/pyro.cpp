// Load Wi-Fi library
#include <WiFi.h>
#include "sensors.h"
#include "pyro.h"

// WIFI credentials
const char* ssid     = "Test"; //custom ssid
const char* password = "Test"; //custom pw

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String pyroState = "off";

void setupWIFI() {
  // Initialize the output variables as outputs
  pinMode(PYRO_CHANNEL_PIN, OUTPUT);
  // Set outputs to LOW
  digitalWrite(PYRO_CHANNEL_PIN, LOW);

  // Set up AP Mode
  Serial.print("Setting AP...");
  WiFi.softAP(ssid, password);

  // Print local IP address and start web server
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void WIFIloop(){
  WiFiClient client = server.available();
  // Start connection
  client.connected();
  Serial.println("New client connected.");
  header += client.read();
  // Turns the EMatch on and off
  if (header.indexOf("GET /12/on") >= 0) {
    Serial.println("EMatch on");
    pyroState = "on";
    digitalWrite(PYRO_CHANNEL_PIN, HIGH);
  } else if (header.indexOf("GET /12/off") >= 0) {
    Serial.println("EMatch off");
    pyroState = "off";
    digitalWrite(PYRO_CHANNEL_PIN, LOW);
  }
            
  // Display the HTML web page
  Serial.println("Displaying HTML");
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS to style the on/off buttons 
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #FFFFFF; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  
  // Web Page Heading
  client.println("<body><h1>ESP32 Web Server</h1>");
  
  // Display current state, and ON/OFF buttons for EMatch  
  client.println("<p>EMatch - State " + pyroState + "</p>");
  // If the pyroState is off, it displays the ON button       
  if (pyroState=="off") {
    client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
  } 
          
  client.println("</body></html>");
            
  // Clear the header variable
  header = "";
  // Close the connection
  client.stop();
  Serial.println("Client disconnected.");
}
