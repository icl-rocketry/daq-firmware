// Load Wi-Fi library
#include <WiFi.h>
#include "WiFiInterface.h"
#include "daq_pins.h"
#include "../sensorLogging/sensors.h"
// WIFI credentials
const char *ssid = "ICLR_DAQ";
const char *password = "rocketsAreCool!";

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String dispState = "off"; // Kiryl
bool dispTurnedOn = false;

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

bool WIFIloop(bool logging)
{
  /*
  Loop to be run every update whenever the WiFi interface needs to be running
  Takes single input, whether the system is in logging mode
  Returns true if the system should be in logging mode, false if not
  */
  WiFiClient client = server.available();
  bool loggingEnable = logging; // Set initial value on whether to be in logging from status quo
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

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta http-equiv=\"refresh\" CONTENT=\"5\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #00FF00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #FF0000;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Le DAQ Control Panel</h1>");

            // Display current state, and ON/OFF buttons for EMatch
            if (logging) {
              client.println("<p>Logging - ON</p>");
            } else {
              client.println("<p>Logging - OFF</p>");
            }
            client.println("<p>Data Display - State " + dispState + "</p>");

            if (dispState == "off")
            {
              client.println("<p><a href=\"/disp/on\"><button class=\"button\">Disp On</button></a></p>");
            }
            else
            {
              double outputArray[14];
              double *oup = outputArray;
              readDispData(oup);

              client.println("<p>Temperatures, Internal: " + String(*oup) + "</p>");
              oup++;
              for (int i = 0; i < 4; i++)
              {
                client.println("<p>Thermo" + String(i + 1) + ": " + String(*oup) + "</p>");
                oup++;
              }
              client.println("<p>Pressures, PTAP1: " + String(*oup) + "</p>");
              oup++;
              for (int i = 0; i < 4; i++)
              {
                client.println("<p>PTAP" + String(i + 2) + ": " + String(*oup) + "</p>");
                oup++;
              }
              client.println("<p>Loads:</p>");
              for (int i = 0; i < 4; i++)
              {
                client.println("<p>Load" + String(i + 1) + ": " + String(*oup) + "</p>");
                oup++;
              }

              // Display the button
              client.println("<p><a href=\"/disp/off\"><button class=\"button\">Disp Off</button></a></p>");
            }
            
            // If the system is not in logging mode, it displays the ON button
            if (!logging)
            {
              client.println("<p><a href=\"/logging/on\"><button class=\"button\">Logging Mode On</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/logging/off\"><button class=\"button button2\">Logging Mode Off</button></a></p>");
            }
            
            if (header.indexOf("GET /status") >= 0)
            {
              client.print("Ematch Status: ");
              client.println(digitalRead(PYRO_CHANNEL_PIN));

              client.print("Continuity Status: ");
              client.println(digitalRead(PYRO_CHANNEL_CONT));

              break;
            }

            if (header.indexOf("GET /logging") >= 0)
            {
              // Turns the EMatch on and off
              if (header.indexOf("GET /logging/on") >= 0)
              {
                Serial.println("Logging on");
                loggingEnable = true;

                client.print("<HEAD>");
                client.print("<meta http-equiv=\"refresh\" content=\"0;url=/\">");
                client.print("</head>");
              }
              else if (header.indexOf("GET /logging/off") >= 0)
              {
                Serial.println("Logging off");
                loggingEnable = false;

                client.print("<HEAD>");
                client.print("<meta http-equiv=\"refresh\" content=\"0;url=/\">");
                client.print("</head>");
              }
            }

            if (header.indexOf("GET /disp") >= 0)
            {
              if (header.indexOf("GET /disp/on") >= 0)
              {
                Serial.println("Display On");
                dispState = "on";
                dispTurnedOn = true;

                client.print("<HEAD>");
                client.print("<meta http-equiv=\"refresh\" content=\"0;url=/\">");
                client.print("</head>");
              }
              else if (header.indexOf("GET /disp/off") >= 0)
              {
                Serial.println("Display Off");
                dispState = "off";
                dispTurnedOn = false;

                client.print("<HEAD>");
                client.print("<meta http-equiv=\"refresh\" content=\"0;url=/\">");
                client.print("</head>");
              }
            }
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();

            // Break out of the while loop
            break;
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
  return loggingEnable;
}
