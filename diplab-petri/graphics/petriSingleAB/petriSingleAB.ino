/* MIT License 2023 Matteo Subet <hi@zumat.ch>
   
   Beta Version 0.2 - DiPLaB
   Spearhead Research Project - https://spearhead-amr.github.io/makeaware/
   
        Coded by Matteo Subet - Last version of 23 August 2023
   
   SUPSI - University of Applied Sciences and Arts of Southern Switzerland
   DACD - Department of Architecture, Construction and Design
   IDe - Design Institute

   Based on Inkplate 6PLUS (ESP32).
   You can find the complete repository here: https://github.com/zumatt/diplab
*/

/*
    -----------------------------------------------------------
                            BOARD CHECK
    -----------------------------------------------------------
*/
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

/*
    -----------------------------------------------------------
                            LIBRARIES
    -----------------------------------------------------------
*/
#include "Inkplate-mod.h"
/*
    -----------------------------------------------------------
                            VARIABLES
    -----------------------------------------------------------
*/
//Variables to display stuff on the E-Ink display
int dispW = 1024;                                           //Display Width
int dispH = 758;                                            //Display Height
int petriD = (dispH/2)-10;                                  //Petri dish diameter
int xC =  (dispW/2)-(petriD);                               //X position of left side of petri dish
int yC =  (dispH/2)-(petriD);                               //Y position of top side of petri dish
int xC2 = (dispW/2)+(petriD);                               //X position of right side of petri dish
int yC2 = (dispH/2)+(petriD);                               //Y position of bottom side of petri dish
int yPos = dispH/2;                                         //Center y of petri dish
int xPos = dispW/2;                                         //Center x of petri dish
int abDiameter = 35;                                        //Dimension of the AB circle
double ab_resistance_multiplier = 0.25;                     //Multiplier from pixels to mm for ab resistance
double ab1_resistance = ab_resistance_multiplier * 30;      //Dimension of the AB resistance in mm
float bacteriaDotsX[500];                                   //Array that contains all the X values of the dots created for the bacteria growth
float bacteriaDotsY[500];                                   //Array that contains all the Y values of the dots created for the bacteria growth

/*
    -----------------------------------------------------------
                    FUNCTIONS TO SETUP THE BOARD
    -----------------------------------------------------------
*/

// Create an object on Inkplate library and also set library into 1 Bit mode (BW)
Inkplate display(INKPLATE_1BIT);


/*
    -----------------------------------------------------------
                          SETUP FUNCTION
    -----------------------------------------------------------
*/

void setup() {
  //Serial communication open to test
  Serial.begin(115200);
  
  //Begin and clear display
  display.begin();
  display.clearDisplay();
  display.fillScreen(BLACK);
  display.display();

  //Display settings
  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setTextWrap(true);

    //Create all the dots for the display of bacteria growing
    /*
  for(int i=0; i<500; i++){
    bacteriaDotsX[i] = random(xC, xC2);
    bacteriaDotsY[i] = random(yC, yC2);
    //Serial.print("Bacteria dot created. Coordinates -> X: ");
    //Serial.print(bacteriaDotsX[i]);
    //Serial.print("    -    Y: ");
    //Serial.println(bacteriaDotsY[i]);
  }
  for(int i=0; i<500; i++){
    display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 4, WHITE);
  }
  Serial.println("Dots created!");*/
  Serial.print(ab1_resistance);
    display.fillCircle(dispW/2, dispH/2, ab1_resistance*abDiameter, WHITE);
  display.fillCircle(dispW/2, dispH/2, abDiameter, BLACK);
  //display.setCursor(dispW/2 - abDiameter/2, dispH/2 + abDiameter/3);
  //isplay.println("1");
  display.display();
}

void loop() {}
