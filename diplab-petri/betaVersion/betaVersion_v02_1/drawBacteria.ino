/*
    -----------------------------------------------------------
                          DRAW BACTERIA
    -----------------------------------------------------------
*/

void drawingLoop(){
  for(int i=0; i<=lenArrY; i++){
    int a;
    double b;
    //This If statement is required to convert the for loop value into the two other values that I need
    if (i==0){a=-10;b=-1.0;}
      else if (i==1){a=-9;b=-0.9;}
      else if (i==2){a=-8;b=-0.8;}
      else if (i==3){a=-7;b=-0.7;}
      else if (i==4){a=-6;b=-0.6;}
      else if (i==5){a=-5;b=-0.5;}
      else if (i==6){a=-4;b=-0.4;}
      else if (i==7){a=-3;b=-0.3;}
      else if (i==8){a=-2;b=-0.2;}
      else if (i==9){a=-1;b=-0.1;}
      else if (i==10){a=0;b=0.0;}
      else if (i==11){a=1;b=0.1;}
      else if (i==12){a=2;b=0.2;}
      else if (i==13){a=3;b=0.3;}
      else if (i==14){a=4;b=0.4;}
      else if (i==15){a=5;b=0.5;}
      else if (i==16){a=6;b=0.6;}
      else if (i==17){a=7;b=0.7;}
      else if (i==18){a=8;b=0.8;}
      else if (i==19){a=9;b=0.9;}
      else if (i==20){a=10;b=1.0;}
      else if (i==21){a=11;b=1.1;}
    drawBacteria_y(i, a, b); //Draw rect function
  }

  for(int h=0; h<=lenArrX; h++){
    int a;
    double b;
    //This If statement is required to convert the for loop value into the two other values that I need
    if (h==0){a=-10;b=-1.0;}
      else if (h==1){a=-9;b=-0.9;}
      else if (h==2){a=-8;b=-0.8;}
      else if (h==3){a=-7;b=-0.7;}
      else if (h==4){a=-6;b=-0.6;}
      else if (h==5){a=-5;b=-0.5;}
      else if (h==6){a=-4;b=-0.4;}
      else if (h==7){a=-3;b=-0.3;}
      else if (h==8){a=-2;b=-0.2;}
      else if (h==9){a=-1;b=-0.1;}
      else if (h==10){a=0;b=0.0;}
      else if (h==11){a=1;b=0.1;}
      else if (h==12){a=2;b=0.2;}
      else if (h==13){a=3;b=0.3;}
      else if (h==14){a=4;b=0.4;}
      else if (h==15){a=5;b=0.5;}
      else if (h==16){a=6;b=0.6;}
      else if (h==17){a=7;b=0.7;}
      else if (h==18){a=8;b=0.8;}
      else if (h==19){a=9;b=0.9;}
      else if (h==20){a=10;b=1.0;}
      else if (h==21){a=11;b=1.1;}
    drawBacteria_x(h, a, b); //Draw rect function
  }
}

void drawBacteria_y(int j, int k, double w){
  //Statement that check the accellerometer Position and if the line was already drawn or not
  if(::arrY[j] == 0 && accY >= (0.0+w) && accY <= (0.09+w)){
      //Function to draw the rectangle
      display.fillRect(::xC, ::yPos-(k*::step)-(bactRect/2 + 10), bactRect, bactRect*2 + 10, BLACK);
      for (int i=2; i<=40; i+=2){display.fillRect(::xC + (bactRect * i), ::yPos-(k*::step)-(bactRect/2 + 10), bactRect, bactRect*2 + 10, BLACK);}
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
      //Function to draw the rectangle
      display.fillRect(::xPos+(k*::step)+(bactRect/2), 0, bactRect, dispH, BLACK);
      //Partial update to the screen (check that this works only with BLACK / WHITE color and not with 0 / 1
      display.partialUpdate();
      ::arrX[j] = 1;
      //Serial.print("!!!!!!!!!! -> Bacteria line drawn on X axis! Position ");
      //Serial.print(j-10);
      //Serial.println(" x !");
  }  
}

void checkLoopSpreading(){
  if(arrY[0]+arrY[1]+arrY[2]+arrY[3]+arrY[4]+arrY[5]+arrY[6]+arrY[7]+arrY[8]+arrY[9]+arrY[10]+arrY[11]+arrY[12]+arrY[13]+arrY[14]+arrY[15]+arrY[16]+arrY[17]+arrY[18]+arrY[19]+arrY[20] == 21){
     arrYcompleted = true;
  } else {
     arrYcompleted = false;
  }

  if(arrX[0]+arrX[1]+arrX[2]+arrX[3]+arrX[4]+arrX[5]+arrX[6]+arrX[7]+arrX[8]+arrX[9]+arrX[10]+arrX[11]+arrX[12]+arrX[13]+arrX[14]+arrX[15]+arrX[16]+arrX[17]+arrX[18]+arrX[19]+arrX[20] == 21){
     arrXcompleted = true;
  } else {
     arrXcompleted = false;
  }

  if (arrXcompleted == true && arrYcompleted == true){
    Serial.println("Everything is covered!");
    arrayCompletedSpreading = true;
  }
}