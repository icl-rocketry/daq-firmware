// Load Wi-Fi library
#include <WiFi.h>
#include "WiFiButton.h"
#include "daq_pins.h"

// WIFI credentials
const char *ssid = "ICLR_DAQ";
const char *password = "rocketsAreCool!";

// Variable to store the HTTP request
String header;

String pyroState;

// Set web server port number to 80
WiFiServer server(80);

void setupWIFI()
{
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

int WIFIloop(int currStateID)
{
  WiFiClient client = server.available();
  int stateID = 0;

  // Start connection
  if (client)
  {
    Serial.println("New client.");
    String currentLine = "";

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.print(c);
        header += c;

        if (c == '\n')
        {
          // Send HTTP response
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /pyro") >= 0)
            {
              // Switch EMatch between idle, calibration and logging states
              if (header.indexOf("GET /pyro/idle") >= 0)
              {
                Serial.println("EMatch in idle state");
                pyroState = "Idle";
                stateID = 1;
              }
              else if (header.indexOf("GET /pyro/cal") >= 0)
              {
                Serial.println("EMatch in calibration state");
                pyroState = "Calibration";
                stateID = 2;
              }

              else if (header.indexOf("GET /pyro/log") >= 0)
              {
                Serial.println("EMatch in logging state");
                pyroState = "Logging";
                stateID = 3;
              }

              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the two buttons
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              // Calibration button (green)
              client.println(".button { background-color: #00FF00; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              // Idle button (blue)
              client.println(".button2 {background-color: #0000FF;}"); 
              // Logging button (red)
              client.println(".button3 {background-color: #FF0000;}</style></head>");

              // Web Page Heading
              client.println("<body><h1>ESP32 Web Server</h1>");

              // Display current state, and ON/OFF buttons for EMatch
              client.println("<p>EMatch - State " + pyroState + "</p>");

              // If the pyroState is off, it displays the ON button
              if (currStateID == 1)
              {
                client.println("<p><a href=\"/pyro/cal\"><button class=\"button\">CALIBRATION</button></a></p>");
                client.println("<p><a href=\"/pyro/log\"><button class=\"button button3\">LOGGING</button></a></p>");
              }
              else if (currStateID == 2)
              {
                client.println("<p><a href=\"/pyro/idle\"><button class=\"button button2\">IDLE</button></a></p>");
                client.println("<p><a href=\"/pyro/log\"><button class=\"button button3\">LOGGING</button></a></p>");
              }
              else if (currStateID == 3)
              {
                client.println("<p><a href=\"/pyro/cal\"><button class=\"button\">CALIBRATION</button></a></p>");
                client.println("<p><a href=\"/pyro/idle\"><button class=\"button button2\">IDLE</button></a></p>");
              }
              
              client.println("</body></html>");

              // The HTTP response ends with another blank line
              client.println();

              // Break out of the while loop
              break;
            }
            else if (header.indexOf("GET /status") >= 0)
            {
              client.print("Ematch Status: ");
              client.println(pyroState);

              client.print("Continuity Status: ");
              client.println(digitalRead(PYRO_CHANNEL_CONT));

              break;
            }
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        { // if you got anything else but a carriage return character,
          currentLine += c;
        }
      }
    }
    client.println("</body></html>");

    // Clear the header variable
    header = "";

    // Close the connection
    Serial.println();
    Serial.println("Disconnecting...");
    client.stop();
  }
  return stateID;
}
