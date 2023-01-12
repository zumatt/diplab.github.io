void microscopeMode(){
    counter = 0;
    baseLayer();

      display.fillCircle(xC2-100, yC+150, 50, 0);
    display.fillCircle(xC2-100, yC+150, 25, 1);
      display.fillCircle(xC+150, yC+450, 100, 0);
    display.fillCircle(xC+150, yC+450, 25, 1);
      display.fillCircle(xC+400, yC+600, 200, 0);
    display.fillCircle(xC+400, yC+600, 25, 1);
    display.display();
}

void historyMode(){
    counter = 0;
    baseLayer();

    display.fillCircle(xC2-100, yC+150, 25, 1);
    display.fillCircle(xC+150, yC+450, 25, 1);
    display.fillCircle(xC+400, yC+600, 25, 1);
    display.display();
}

void baseLayer(){
    display.clearDisplay();
      //Draw base layer
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    for(int i=1; i<=20; i++){  
      display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}
    for(int a=1; a<=20; a++){  
      display.drawLine(xC, yC+(a*petriD/10), xC2, yC+(a*petriD/10), BLACK);}
    //END of draw base layer
}

void historyBtn(int j,int k,int l){
    baseLayer();

      display.fillCircle(xC2-100, yC+150, 25*j, 0);
    display.fillCircle(xC2-100, yC+150, 25, 1);
      display.fillCircle(xC+150, yC+450, 25*k, 0);
    display.fillCircle(xC+150, yC+450, 25, 1);
      display.fillCircle(xC+400, yC+600, 25*l, 0);
    display.fillCircle(xC+400, yC+600, 25, 1);
    display.partialUpdate();
}

void historyCondition(){
  if(counter == 0){ //0h
    Serial.println("History mode: 0h !");
    historyMode();
  } else if(counter == 1){ //8h
    Serial.println("History mode: 8h !");
    historyBtn(1.3,2,4);
  } else if(counter == 2){ //12h
    Serial.println("History mode: 12h !");
    historyBtn(1.6,3,6);
  } else if(counter == 3){ //24h
    Serial.println("History mode: 24h !");
    historyBtn(2,4,8);
  }
}

void microscopeCondition(){
  if(counter == 0){ //0x
    Serial.println("Microscope mode: 0x !");
    microscopeMode();
  } else if(counter == 1){ //4x
    Serial.println("Microscope mode: 4x !");
    baseLayer();
      display.fillCircle(dispW/2, dispH/2, 100, 0);
    display.fillCircle(dispW/2, dispH/2, 25, 1);
    display.display();
  } else if(counter == 2){ //10x
    Serial.println("Microscope mode: 10x !");
    baseLayer();
      display.fillCircle(dispW/2, dispH/2, 200, 0);
    display.fillCircle(dispW/2, dispH/2, 50, 1);
    display.display();
  } else if(counter == 3){ //40
    Serial.println("Microscope mode: 40x !");
    baseLayer();
    for(int i=0; i<=500; i++){
      display.fillCircle(random(xC, xC2), random(yC, yC2), 2, 1);
    }
      display.fillCircle(dispW/2, dispH/2, 300, 0);
    display.fillCircle(dispW/2, dispH/2, 75, 1);
    display.display();
  }
}