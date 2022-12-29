/*
   First example of creating a web page to select bacteria and store the value inside the ESP32.
   
   29 December 2022 by Matteo Subet
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

#include "Inkplate.h"   //Include Inkplate library to the sketch
#include "htmlCode.h"   //Include .h file where we stored out html code of our web page
#include <WebServer.h>  //Include ESP32 library for Web server
#include <WiFi.h>       //Include ESP32 WiFi library
#include <WiFiClient.h> //Include ESP32 WiFi library for AP
#include <uri/UriBraces.h>

#define ssid "DiPLab"
#define pass "password"

Inkplate display(INKPLATE_1BIT); // Create an object on Inkplate library and also set library into 1 Bit mode (BW)
WebServer server(80);            // Create Web server on port 80 (HTTP port number)

IPAddress serverIP;
String bacteria;

void setup()
{
    Serial.begin(115200);
    
    display.begin();                    // Init Inkplate library (you should call this function ONLY ONCE)
    display.clearDisplay();             // Clear frame buffer of display
    display.display();                  // Put clear image on display
    display.setTextSize(3);             // Scale text to be two times bigger then original (5x7 px)
    display.setTextColor(BLACK, WHITE); // Set text color to black and background color to white
    display.setTextWrap(true);          // If text does not fit on screen, send it to new line

    WiFi.begin();            // Init. WiFi library
    WiFi.mode(WIFI_AP);      // Set WiFi to Access point mode
    WiFi.softAP(ssid, pass); // Set SSID (WiFi name) and password for Access point

    serverIP = WiFi.softAPIP(); // Get the server IP address

    server.on("/", handleRoot); // If you open homepage, go to handle root function
    server.on(UriBraces("/selection/{}"),
              handleString); // If you send some text to Inkplate, go to handleString function. Note that {} brackets at
                             // the end of address. That means that web address has some arguments (our text!).
    server.begin();          // Start the web server
    updatePaper();
}

void loop()
{
    server.handleClient(); // You have to constantly read if there is any new client connected to web server
}

void updateHTML()
{ // This function will send response to client and send HTML code of our web page
    server.send(200, "text/html", s);
}

void handleRoot()
{ // This function will send response to client if client open a root (homepage) of our web page
    updateHTML();
}

void handleString()
{ // This function will send response to client, send HTML code of web page, get the text from argument sent in web page
  // address and refresh screen with new text
    bacteria = server.arg("bacteria");
    updateHTML();
    updatePaper();
}

void updatePaper()
{                              // This function updates screen with new data (text)
    display.clearDisplay();    // Clear everything from epaper frame buffer
    display.setCursor(20, 40); // Print out instruction on how to connect to Inkplate WiFi and how to open a web page
    display.print("Connect to ");
    display.print(ssid);
    display.println(" WiFi with pass: ");
    display.setCursor(240, 100);
    display.println(pass);
    display.setCursor(100, 150);
    display.print("Open Your web browser and open");
    display.setCursor(240, 210);
    display.print("http://");
    display.print(serverIP);
    display.println('/');
    display.println();
    display.fillRect(10, 240, 780, 4, BLACK);
    display.println("Selected bacteria:"); // Print out what user typed in web page
    display.print(bacteria);
    display.display(); // Send everything to screen (refresh the screen)
}
