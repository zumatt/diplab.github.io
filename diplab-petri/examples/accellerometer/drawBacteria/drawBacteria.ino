/* MIT License 2022 Matteo Subet <hi@zumat.ch>
   First draft of accellerometer drawing on the E-Ink display.

   This is working by connecting to the I2C port the Grove accellerometer.
   The expected output will be to see on the E-Ink display a circle and then by moving the accellerometer lot of lines that appears
   while moving the accellerometer in X or Y.

   This is working using a partial update of the E-Ink screen but require the display to be at 1BIT (BLACK or WHITE).
   To work with grey you need to change the color code into 3BIT and you need to refresh each time the whole display with display.display().
   
   To fix what is written here on top, is just required to write 0 as WHITE and 1 as BLACK.
   28th December 2022 by Matteo Subet
*/

#include "Inkplate.h"
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
int arrY[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Array of Y axis for checking if a line was drawn
int lenArrY = *(&arrY + 1) - arrY;                        //Array of Y axis length
int arrX[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Array of X axis for checking if a line was drawn
int lenArrX = *(&arrX + 1) - arrX;                        //Array of X axis length
double accX, accY;                                        //Variables to store accellerometer data
int yPos = dispH/2;                                       //Center y of petri dish
int xPos = dispW/2;                                       //Center x of petri dish
int step = petriD/10;                                     //Step of each line of bacteria

void setup() {
  Serial.begin(115200);
  display.begin();                                     //Start the display
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);     //Draw petri circle
  display.display();                                   //Display what was drawn

  LIS.begin(WIRE,0x19); //IIC init
  delay(100);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

}

void loop() {
  accX = LIS.getAccelerationX(); //Get accellerometer X data
  accY = LIS.getAccelerationY(); //Get accellerometer Y data
  //Serial.print("Accellerometer X: "); Serial.print(accX); Serial.print("      ---      Accellerometer Y: "); Serial.println(accY);
  
  for(int i=0; i<=lenArrY; i++){
    int a;
    double b;
    //This If statement is required to convert the for loop value into the two other values that I need
    if (i==0){a=-10;b=-1.0;}
      else if (i==1){a=-9;b=-0.9;}
      else if (i==2){a=-8;b=-0.8;}
      else if (i==3){a=-7;b=-0.7;}
      else if (i==4){a=-6;b=-0.6;} else if (i==5){a=-5;b=-0.5;} else if (i==6){a=-4;b=-0.4;} else if (i==7){a=-3;b=-0.3;} else if (i==8){a=-2;b=-0.2;} else if (i==9){a=-1;b=-0.1;} else if (i==10){a=0;b=0.0;} else if (i==11){a=1;b=0.1;} else if (i==12){a=2;b=0.2;} else if (i==13){a=3;b=0.3;} else if (i==14){a=4;b=0.4;} else if (i==15){a=5;b=0.5;} else if (i==16){a=6;b=0.6;} else if (i==17){a=7;b=0.7;} else if (i==18){a=8;b=0.8;} else if (i==19){a=9;b=0.9;} else if (i==20){a=10;b=1.0;}
    drawBacteria_y(i, a, b); //Draw line function
  }

  for(int h=0; h<=lenArrX; h++){
    int a;
    double b;
    //This If statement is required to convert the for loop value into the two other values that I need
    if (h==0){a=-10;b=-1.0;}
      else if (h==1){a=-9;b=-0.9;}
      else if (h==2){a=-8;b=-0.8;}
      else if (h==3){a=-7;b=-0.7;}
      else if (h==4){a=-6;b=-0.6;} else if (h==5){a=-5;b=-0.5;} else if (h==6){a=-4;b=-0.4;} else if (h==7){a=-3;b=-0.3;} else if (h==8){a=-2;b=-0.2;} else if (h==9){a=-1;b=-0.1;} else if (h==10){a=0;b=0.0;} else if (h==11){a=1;b=0.1;} else if (h==12){a=2;b=0.2;} else if (h==13){a=3;b=0.3;} else if (h==14){a=4;b=0.4;} else if (h==15){a=5;b=0.5;} else if (h==16){a=6;b=0.6;} else if (h==17){a=7;b=0.7;} else if (h==18){a=8;b=0.8;} else if (h==19){a=9;b=0.9;} else if (h==20){a=10;b=1.0;}
    drawBacteria_x(h, a, b); //Draw line function
  }
}

void drawBacteria_y(int j, int k, double w){
  //Statement that check the accellerometer Position and if the line was already drawn or not
  if(::arrY[j] == 0 && accY >= (0.0+w) && accY <= (0.09+w)){
      //Function to draw the line
      display.drawLine(::xC, ::yPos-(k*::step), ::xC2, ::yPos-(k*::step), BLACK);
      //Partial update to the screen (check that this works only with BLACK / WHITE color and not with 0 / 1
      display.partialUpdate();
      ::arrY[j] = 1;
      //Serial.print("!!!!!!!!!! -> Bacteria line drawn on Y axis! Position ");
      //Serial.print(j-10);
      //Serial.println(" y !");
  }  
}
void drawBacteria_x(int j, int k, double w){
  //Statement that check the accellerometer Position and if the line was already drawn or not
  if(::arrX[j] == 0 && accX >= (0.0+w) && accX <= (0.09+w)){
      //Function to draw the line
      display.drawLine(::xPos+(k*::step), ::yC, ::xPos+(k*::step), ::yC2, BLACK);
      //Partial update to the screen (check that this works only with BLACK / WHITE color and not with 0 / 1
      display.partialUpdate();
      ::arrX[j] = 1;
      //Serial.print("!!!!!!!!!! -> Bacteria line drawn on X axis! Position ");
      //Serial.print(j-10);
      //Serial.println(" x !");
  }  
}