/* MIT License 2023 Matteo Subet <hi@zumat.ch>
   
   Beta Version 0.2 - DiPLaB
   Spearhead Research Project - https://spearhead-amr.github.io/makeaware/
   
        Coded by Matteo Subet - Last version of 22 August 2023
   
   SUPSI - University of Applied Sciences and Arts of Southern Switzerland
   DACD - Department of Architecture, Construction and Design
   IDe - Design Institute

   Based on Inkplate 6PLUS (ESP32)
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

#include "Inkplate-mod.h"
#include "Fonts/FreeSansBold9pt7b.h"

const GFXfont *textFont = &FreeSansBold9pt7b;

Inkplate display(INKPLATE_1BIT);

int antibioticCreated = 0;                                      //Numbers of antibiotic placed
int abDiameter = 35;                                        //Dimension of the AB circle
int prevCoord[4] = {0,0,0,0};                               //Previous coordinates
int differenceCoordX1 = 0;                                  //Variable to store the difference between the current variable and the old one for X position of AB 1
int differenceCoordY1 = 0;                                  //Variable to store the difference between the current variable and the old one for Y position of AB 1
int differenceCoordX2 = 0;                                  //Variable to store the difference between the current variable and the old one for X position of AB 2
int differenceCoordY2 = 0;                                  //Variable to store the difference between the current variable and the old one for Y position of AB 2
int tresholdAntibiotic = 150;                               //Threshold between two anbiotics pills

// If your Inkplate doesn't have external (or second) MCP I/O expander, you should uncomment next line,
// otherwise your code could hang out when you send code to your Inkplate.
// You can easily check if your Inkplate has second MCP by turning it over and 
// if there is missing chip near place where "MCP23017-2" is written, but if there is
// chip soldered, you don't have to uncomment line and use external MCP I/O expander

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    display.begin();
    display.fillScreen(BLACK);
    display.setFont(textFont);
    display.setTextSize(3);
    display.setTextColor(BLACK, WHITE);
    display.setTextWrap(true);
    display.display();

    pinMode(36, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(39, INPUT_PULLUP);  // Enable internal pull-up resistor

    // Init touchscreen and power it on after init (send false as argument to put it in deep sleep right after init)
    if (display.tsInit(true))
    {
        Serial.println("Touchscreen init ok");
    }
    else
    {
        Serial.println("Touchscreen init fail");
        while (true);
    }
}

void loop()
{
    // Check if there is any touch detected
    if (display.tsAvailable())
    {
        if (antibioticCreated < 3){
          uint8_t n;
          uint16_t x[2], y[2];
          // See how many fingers are detected (max 2) and copy x and y position of each finger on touchscreen
          n = display.tsGetData(x, y);
          if (n != 0)
          {
            if (antibioticCreated == 1){
              differenceCoordX1 = prevCoord[0] - x[0];
              differenceCoordY1 = prevCoord[1] - y[0];
            } else if (antibioticCreated == 2){
              differenceCoordX2 = prevCoord[2] - x[0];
              differenceCoordY2 = prevCoord[3] - y[0];
              differenceCoordX1 = prevCoord[0] - x[0];
              differenceCoordY1 = prevCoord[1] - y[0];
            }            
            
            if ((!(differenceCoordX1 >= -tresholdAntibiotic && differenceCoordX1 <= tresholdAntibiotic) ||
                 !(differenceCoordY1 >= -tresholdAntibiotic && differenceCoordY1 <= tresholdAntibiotic)) ||
                 (differenceCoordX1 == 0 && differenceCoordY1 == 0)) {
                    
                    if ((!(differenceCoordX2 >= -tresholdAntibiotic && differenceCoordX2 <= tresholdAntibiotic) ||
                         !(differenceCoordY2 >= -tresholdAntibiotic && differenceCoordY2 <= tresholdAntibiotic)) ||
                         (differenceCoordX2 == 0 && differenceCoordY2 == 0)) {
                            display.fillCircle(x[0], y[0], abDiameter, WHITE);
                            display.setCursor(x[0] - abDiameter/2, y[0] + abDiameter/3);
                            if (antibioticCreated == 0) {display.println("1");} else if (antibioticCreated == 1){display.println("2");} else if (antibioticCreated == 2){display.println("3");}
                            display.partialUpdate();
                            if (antibioticCreated == 0){
                              prevCoord[0] = x[0];
                              prevCoord[1] = y[0];
                            } else if (antibioticCreated == 1){
                              prevCoord[2] = x[0];
                              prevCoord[3] = y[0];
                            }
                            antibioticCreated += 1;
                            Serial.println(antibioticCreated);
                        } else {
                          Serial.println("Error generating AB - Code 2");
                          errorCheck(x[0], y[0]);}
                 } else {
                          Serial.println("Error generating AB - Code 1");
                          errorCheck(x[0], y[0]);}
          }
        } else if (antibioticCreated == 3){
          Serial.println("Antibiotic placed correctly");
          Serial.println(antibioticCreated);
          // From here - delete is only for testing
          sleep(4);
          antibioticCreated = 0;
          differenceCoordX1 = 0;
          differenceCoordY1 = 0;
          differenceCoordX2 = 0;
          differenceCoordY2 = 0;
          display.fillScreen(BLACK);
          display.display();
          // To here - delete is only for testing
        }
    }
}

void errorCheck(float xCoord, float yCoord){
  Serial.print("Previous coordinate X: ");
  Serial.println(prevCoord[0]);
  Serial.print("Previous coordinate Y: ");
  Serial.println(prevCoord[1]);
  Serial.print("Coordinate X: ");
  Serial.println(xCoord);
  Serial.print("Coordinate Y: ");
  Serial.println(yCoord);
  Serial.print("Calc X 1: ");
  Serial.println(differenceCoordX1);
  Serial.print("Calc Y 1: ");
  Serial.println(differenceCoordY1);
  Serial.print("Calc X 2: ");
  Serial.println(differenceCoordX2);
  Serial.print("Calc Y 2: ");
  Serial.println(differenceCoordY2);
}