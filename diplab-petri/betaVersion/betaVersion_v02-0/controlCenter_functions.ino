/*
    -----------------------------------------------------------
                          CONTROL CENTER
    -----------------------------------------------------------
*/

void readingMode(){
    display.clearDisplay();
    display.fillScreen(BLACK);
    display.display();

    if (j_readingAB == 1){
      readingAbVal = j_ab1;
        for(int i=0; i<500; i++){
          display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 1.5, WHITE);
        }

          display.fillCircle(dispW/2, dispH/2, ab1_resistance*abDiameter, BLACK);
        display.fillCircle(dispW/2, dispH/2, abDiameter, 1);
        display.setCursor(dispW/2 - abDiameter/2, dispH/2 + abDiameter/3);
        display.println("1");
        display.display();
    } else if (j_readingAB == 2){
      readingAbVal = j_ab2;
        for(int i=0; i<500; i++){
          display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 1.5, WHITE);
        }

          display.fillCircle(dispW/2, dispH/2, ab2_resistance*abDiameter, BLACK);
        display.fillCircle(dispW/2, dispH/2, abDiameter, 1);
        display.setCursor(dispW/2 - abDiameter/2, dispH/2 + abDiameter/3);
        display.println("2");
        display.display();
    } else if (j_readingAB == 3){
      readingAbVal = j_ab3;
        for(int i=0; i<500; i++){
          display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 1.5, WHITE);
        }

          display.fillCircle(dispW/2, dispH/2, ab3_resistance*abDiameter, BLACK);
        display.fillCircle(dispW/2, dispH/2, abDiameter, WHITE);
        display.setCursor(dispW/2 - abDiameter/2, dispH/2 + abDiameter/3);
        display.println("3");
        display.display();
    } else {
          Serial.println("Reading AB not selected!");
    }
}

void historyMode(){
    counter = 0;
}

//nDots is the numbers of dots representing bacteria to generate values can be from 0 to 500 max
void historyBase(int nDots){
    display.clearDisplay();
    display.fillScreen(BLACK);
    
    for(int i=0; i<nDots; i++){
      display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 1.5, WHITE);
    }

      display.fillCircle(ab1_x, ab1_y, abDiameter, BLACK);
      display.fillCircle(ab2_x, ab2_y, abDiameter, BLACK);
      display.fillCircle(ab3_x, ab3_y, abDiameter, BLACK);
    display.fillCircle(ab1_x, ab1_y, abDiameter, WHITE);
        display.setCursor(ab1_x - abDiameter/2, ab1_y + abDiameter/3);
        display.println("1");
    display.fillCircle(ab2_x, ab2_y, abDiameter, WHITE);
        display.setCursor(ab2_x - abDiameter/2, ab2_y + abDiameter/3);
        display.println("2");
    display.fillCircle(ab3_x, ab3_y, abDiameter, WHITE);
        display.setCursor(ab3_x - abDiameter/2, ab3_y + abDiameter/3);
        display.println("3");
    display.display();
}

void historyCondition(){
  if(counter == 0){ //0h
    Serial.println("History mode: 0h !");
    historyHours = 0;
    historyMode();
    historyBase(0);
  } else if(counter == 1){ //8h
    Serial.println("History mode: 8h !");
    historyHours = 8;
    historyBase(100);
  } else if(counter == 2){ //12h
    Serial.println("History mode: 12h !");
    historyHours = 12;
    historyBase(250);
  } else if(counter == 3){ //24h
    Serial.println("History mode: 24h !");
    historyHours = 24;
    historyBase(500);
  }
}
//}