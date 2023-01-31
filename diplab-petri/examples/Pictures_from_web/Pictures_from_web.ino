/*
   Web_BMP_pictures example for e-radionica Inkplate 6PLUS
   For this example you will need a micro USB cable, Inkplate 6PLUS, and an available WiFi connection.
   Select "Inkplate 6PLUS(ESP32)" from Tools -> Board menu.
   Don't have "Inkplate 6PLUS(ESP32)" option? Follow our tutorial and add it:
   https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

   You can open .bmp files that have color depth of 1 bit (BW bitmap), 4 bit, 8 bit and
   24 bit AND have resoluton smaller than 800x600 or otherwise it won't fit on screen.

   This example will show you how you can download a .bmp file (picture) from the web and
   display that image on e-paper display.

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
   11 February 2021 by e-radionica.com
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

#include "HTTPClient.h"          //Include library for HTTPClient
#include "Inkplate.h"            //Include Inkplate library to the sketch
#include "WiFi.h"                //Include library for WiFi
Inkplate display(INKPLATE_1BIT); // Create an object on Inkplate library and also set library into 1 Bit mode (BW)

const char ssid[] = "zumat iphone";    // Your WiFi SSID
const char *password = "mattepss"; // Your WiFi password

// If your Inkplate doesn't have external (or second) MCP I/O expander, you should uncomment next line,
// otherwise your code could hang out when you send code to your Inkplate.
// You can easily check if your Inkplate has second MCP by turning it over and 
// if there is missing chip near place where "MCP23017-2" is written, but if there is
// chip soldered, you don't have to uncomment line and use external MCP I/O expander

void setup()
{
    Serial.begin(115200);
    display.begin();        // Init Inkplate library (you should call this function ONLY ONCE)
    display.clearDisplay(); // Clear frame buffer of display
    display.display();      // Put clear image on display

    Serial.print("Connecting to WiFi...");

    // Connect to the WiFi network.
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi OK! Downloading...");

    if (!display.drawImage("https://raw.githubusercontent.com/zumatt/diplab/main/diplab-petri/visualTest/DiPLab-VisualTest-1.bmp", 0, 0, false, true))
    {
        Serial.println("Image open error");
    }
    display.display();

    delay(2000);

    if (!display.drawImage("https://raw.githubusercontent.com/zumatt/diplab/main/diplab-petri/visualTest/DiPLab-VisualTest-2.png", 0, 0, false, true))
    {
        Serial.println("Image open error");
    }
    display.display();

    WiFi.mode(WIFI_OFF);
}

void loop()
{
    // Nothing...
}
