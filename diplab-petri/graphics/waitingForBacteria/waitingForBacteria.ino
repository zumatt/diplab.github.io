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
#include "Fonts/FreeSansBold9pt7b.h"

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
const GFXfont *textFont = &FreeSansBold9pt7b;               //Font setup for FreeSansBold9pt7b

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
  display.display();

  //Display settings
  display.setFont(textFont);
  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setTextWrap(true);

  graphicWaitingBact();
}

void loop() {}

void graphicWaitingBact(){
  //Display all the informations for the WiFi connection
    display.clearDisplay();
  //display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    display.setCursor(xC+90, yC+200);
    display.setTextSize(2);
    display.println("Waiting for bacteria");
    display.setTextSize(1);
    display.setCursor(xC+90, yC+360);
    display.println("select the bacteria with your phone");
    
    for (int i=0; i<5; i++){
      display.fillCircle(xC+640+(i*30), yC + 420, 10, BLACK);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xC+610+(i*30), yC + 450, 10, BLACK);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xC+550+(i*30), yC + 480, 10, BLACK);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xC+490+(i*30), yC + 510, 10, BLACK);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xC+340+(i*30), yC + 540, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+280+(i*30), yC + 570, 10, BLACK);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xC+220+(i*30), yC + 600, 10, BLACK);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xC+100+(i*30), yC + 630, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+130+(i*30), yC + 660, 10, BLACK);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xC+160+(i*30), yC + 690, 10, BLACK);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xC+190+(i*30), yC + 720, 10, BLACK);   
    }
    display.setTextSize(2);
    display.display();
}