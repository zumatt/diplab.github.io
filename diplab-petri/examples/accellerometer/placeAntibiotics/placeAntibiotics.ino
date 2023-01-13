/* MIT License 2022 Matteo Subet <hi@zumat.ch>
   First draft of accellerometer placing antibiotics randomly on the E-Ink display.

   This is working by connecting to the I2C port the Grove accellerometer.
   The expected output will be to see on the E-Ink display a circle with a grid (bacteria) and then by shaking the accellerometer
   3 dots (antibiotics) appear.

   This is working using a total update of the E-Ink screen.
   28th December 2022 by Matteo Subet
*/

#include "Inkplate-mod.h"
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire

Inkplate display(INKPLATE_1BIT);

int dispW = 1024;                                         //Display Width
int dispH = 758;                                          //Display Height
int petriD = (dispH/2)-10;                                //Petri dish diameter
int xC =  (dispW/2)-(petriD);                             //X position of left side of petri dish
int yC =  (dispH/2)-(petriD);                             //Y position of top side of petri dish
int xC2 = (dispW/2)+(petriD);                             //X position of right side of petri dish
int yC2 = (dispH/2)+(petriD);                             //Y position of bottom side of petri dish
double accX, accY;                                        //Variables to store accellerometer data
int yPos = dispH/2;                                       //Center y of petri dish
int xPos = dispW/2;                                       //Center x of petri dish
int ab1_x, ab1_y;                                       //Position of first antibiotic
int ab2_x, ab2_y;                                       //Position of second antibiotic
int ab3_x, ab3_y;                                       //Position of third antibiotic

void setup() {
  Serial.begin(115200);
  display.begin();                                                             //Start the display
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);                         //Draw petri circle
  for(int i=1; i<=19; i++){                                   
    display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}     //Draw grid on Y axis
  for(int a=1; a<=19; a++){  
    display.drawLine(xC, yC+(a*petriD/10), xC2, yC+(a*petriD/10), BLACK);}     //Draw grid on X axis
  display.display();                                                           //Display what was drawn

  LIS.begin(WIRE,0x19); //IIC init
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

}

void loop() {
  accX = LIS.getAccelerationX(); //Get accellerometer X data
  accY = LIS.getAccelerationY(); //Get accellerometer Y data
  //Serial.print("Accellerometer X: "); Serial.print(accX); Serial.print("      ---      Accellerometer Y: "); Serial.println(accY);

  if (accX >= 3.00 || accX <= -3.00 || accY >= 3.00 || accY <= -3.00){      //Accellerometer treshold of shaking is 3.00
    //Serial.println("Shake detected!");
    display.clearDisplay();                                                 //Delete all the things that are shown on the display

    //Redraw base layer
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    for(int i=1; i<=20; i++){  
      display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}
    for(int a=1; a<=20; a++){  
      display.drawLine(xC, yC+(a*petriD/10), xC2, yC+(a*petriD/10), BLACK);}
    //END of redraw base layer

    ab1_x = random(xC+100, xC2-100);
    ab1_y = random(yC+100, yC2-100);
    display.fillCircle(ab1_x, ab1_y, 25, 1);  //First Antibiotic

    ab2_x = random(xC+100, xC2-100);
    ab2_y = random(yC+100, yC2-100);
    display.fillCircle(ab2_x, ab2_y, 25, 1);  //Second Antibiotic

    ab3_x = random(xC+100, xC2-100);
    ab3_y = random(yC+100, yC2-100);
    display.fillCircle(ab3_x, ab3_y, 25, 1);  //Third Antibiotic

    display.partialUpdate();
    delay(1000);               //This delay is needed because of accidental shakes
  }
}