void microscopeMode(){
    counter = 0;
    baseLayer();

      display.fillCircle(ab1_x, ab1_y, abDiameter*ab1_resistance, 0);
      display.fillCircle(ab2_x, ab2_y,  abDiameter*ab2_resistance, 0);
      display.fillCircle(ab3_x, ab3_y,  abDiameter*ab3_resistance, 0);
    display.fillCircle(ab1_x, ab1_y, abDiameter, 1);
    display.fillCircle(ab2_x, ab2_y, abDiameter, 1);
    display.fillCircle(ab3_x, ab3_y, abDiameter, 1);
    display.display();
}

void historyMode(){
    counter = 0;
    baseLayer();

    display.fillCircle(ab1_x, ab1_y, abDiameter, 1);
    display.fillCircle(ab2_x, ab2_y, abDiameter, 1);
    display.fillCircle(ab3_x, ab3_y, abDiameter, 1);
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

void historyBtn(double j,double k,double l){
    baseLayer();

      display.fillCircle(ab1_x, ab1_y, abDiameter*j, 0);
      display.fillCircle(ab2_x, ab2_y, abDiameter*k, 0);
      display.fillCircle(ab3_x, ab3_y, abDiameter*l, 0);
    display.fillCircle(ab1_x, ab1_y, abDiameter, 1);
    display.fillCircle(ab2_x, ab2_y, abDiameter, 1);
    display.fillCircle(ab3_x, ab3_y, abDiameter, 1);
    display.partialUpdate();
}

void historyCondition(){
  if(counter == 0){ //0h
    Serial.println("History mode: 0h !");
    historyMode();
  } else if(counter == 1){ //8h
    Serial.println("History mode: 8h !");
    Serial.print("AB 1: ");
    Serial.print(ab1_res_8h);
    Serial.print(" - AB 2: ");
    Serial.print(ab2_res_8h);
    Serial.print(" - AB 3: ");
    Serial.println(ab3_res_8h);
    historyBtn(ab1_res_8h,ab2_res_8h,ab3_res_8h);
  } else if(counter == 2){ //12h
    Serial.println("History mode: 12h !");
    Serial.print("AB 1: ");
    Serial.print(ab1_res_12h);
    Serial.print(" - AB 2: ");
    Serial.print(ab2_res_12h);
    Serial.print(" - AB 3: ");
    Serial.println(ab3_res_12h);
    historyBtn(ab1_res_12h,ab2_res_12h,ab3_res_12h);
  } else if(counter == 3){ //24h
    Serial.println("History mode: 24h !");
    historyBtn(ab1_res_24h,ab2_res_24h,ab3_res_24h);
  }
}

void microscopeCondition(){
  if(counter == 0){ //0x
    Serial.println("Microscope mode: 0x !");
    microscopeMode();
  } else if(counter == 1){ //4x
    Serial.println("Microscope mode: 4x !");
    baseLayer();
    if (inputMessage8 == "1"){
      display.fillCircle(dispW/2, dispH/2, ab1_resistance*abDiameter, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter, 1);
    display.display();
    } else if (inputMessage8 == "2"){
      display.fillCircle(dispW/2, dispH/2, ab2_resistance*abDiameter, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter, 1);
    display.display();
    } else if (inputMessage8 == "3"){
      display.fillCircle(dispW/2, dispH/2, ab3_resistance*abDiameter, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter, 1);
    display.display();
    } else {
      Serial.println("Microscope AB not selected!");
    }
  } else if(counter == 2){ //10x
    Serial.println("Microscope mode: 10x !");
    baseLayer();
    if (inputMessage8 == "1"){
      display.fillCircle(dispW/2, dispH/2, abDiameter*ab1_resistance*2, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter*2, 1);
    display.display();
    } else if (inputMessage8 == "2"){
      display.fillCircle(dispW/2, dispH/2, abDiameter*ab2_resistance*2, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter*2, 1);
    display.display();
    } else if (inputMessage8 == "3"){
      display.fillCircle(dispW/2, dispH/2, abDiameter*ab3_resistance*2, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter*2, 1);
    display.display();
    } else {
      Serial.println("Microscope AB not selected!");
    }
  } else if(counter == 3){ //40
    Serial.println("Microscope mode: 40x !");
    baseLayer();
    for(int i=0; i<=500; i++){
      display.fillCircle(random(xC, xC2), random(yC, yC2), 2, 1);
    }
    if (inputMessage8 == "1"){
      display.fillCircle(dispW/2, dispH/2, abDiameter*ab1_resistance*3, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter*3, 1);
    display.display();
    } else if (inputMessage8 == "2"){
      display.fillCircle(dispW/2, dispH/2, abDiameter*ab2_resistance*3, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter*3, 1);
    display.display();
    } else if (inputMessage8 == "3"){
      display.fillCircle(dispW/2, dispH/2, abDiameter*ab3_resistance*3, 0);
    display.fillCircle(dispW/2, dispH/2, abDiameter*3, 1);
    display.display();
    } else {
      Serial.println("Microscope AB not selected!");
    }
  }
}

void updateResistanceValue(){
 ab1_res_8h  = ab1_resistance / 3;
 ab1_res_12h = ab1_resistance / 2;
 ab1_res_24h = ab1_resistance;
 ab2_res_8h  = ab2_resistance / 3;
 ab2_res_12h = ab2_resistance / 2;
 ab2_res_24h = ab2_resistance;
 ab3_res_8h  = ab3_resistance / 3;
 ab3_res_12h = ab3_resistance / 2;
 ab3_res_24h = ab3_resistance;
}