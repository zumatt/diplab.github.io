void state0() {
  //Display all the informations for the WiFi connection
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+250);
  display.println("Connect to ");
  display.setCursor(xC+100, yC+300);
  display.print(ssid);
  display.println(" WiFi with pass: ");
  display.setCursor(xC+100, yC+350);
  display.println(password);
  display.setCursor(xC+100, yC+400);
  display.print("Open web browser and write");
  display.setCursor(xC+100, yC+450);
  display.print("http://");
  display.print(serverIP);
  display.println('/');
  display.display();
}

void state1() {
  //Confirm the connection and start an introduction to the exp
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+300);
  display.println("Your device is connected!");
  display.setCursor(xC+100, yC+350);
  display.println("You can start the introduction.");
  display.display();
}

void state2(int k){
  //Introduction spread bacteria
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  if(k == 1){
    for(int i=1; i<=20; i++){  
      display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}
    for(int j=1; j<=20; j++){  
      display.drawLine(xC, yC+(j*petriD/10), xC2, yC+(j*petriD/10), BLACK);}
  display.display();
  } else{
    display.display();
  }
}

void state3(int k){
  //Introduction place antibiotics
    display.clearDisplay();

    //Redraw base layer
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    for(int i=1; i<=20; i++){  
      display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}
    for(int j=1; j<=20; j++){  
      display.drawLine(xC, yC+(j*petriD/10), xC2, yC+(j*petriD/10), BLACK);}
    //END of redraw base layer
    if (k == 1){
    display.fillCircle(random(xC+100, xC2-100), random(xC+100, xC2-100), 25, 1);  //First Antibiotic
    display.fillCircle(random(xC+100, xC2-100), random(xC+100, xC2-100), 25, 1);  //Second Antibiotic
    display.fillCircle(random(xC+100, xC2-100), random(xC+100, xC2-100), 25, 1);  //Third Antibiotic
    display.display();} else {display.display();}
}

void state4(){
  //Introduction history mode

  //AB introduction position
  ab1_x = xC2-100;
  ab1_y = yC+150;
  ab2_x = xC+150;
  ab2_y = yC+450;
  ab3_x = xC+400;
  ab3_y = yC+600;

  //Resistance introduction value
  ab1_resistance = 2;
  ab2_resistance = 4;
  ab3_resistance = 8;

  modality = "history";
  historyMode();
  //Same thing as history mode but with data already inserted
}

void state5(){
  modality = "microscope";
  //Introduction microscope mode
  microscopeMode();
  //Same thing as microscope mode but with data already inserted
}

void state6(){
  //Reset AB position for later
  ab1_x = 0;
  ab1_y = 0;
  ab2_x = 0;
  ab2_y = 0;
  ab3_x = 0;
  ab3_y = 0;

  //Reset resistance value for later
  ab1_resistance = 0;
  ab2_resistance = 0;
  ab3_resistance = 0;

  //Start experience: enter Name and class code
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+250);
  display.println("Your name:");
  display.setCursor(xC+100, yC+300);
  Serial.print("Name from the web : ");
  Serial.println(inputMessage6);
  display.println(inputMessage6);
  display.setCursor(xC+100, yC+400);
  display.print("Your class code:");
  display.setCursor(xC+100, yC+450);
  display.print("#");
  Serial.print("Class code from the web : ");
  Serial.println(inputMessage7);
  display.println(inputMessage7);
  display.display();
}

void state7(){
  //Bacteria selection
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+300);
  display.println(inputMessage2);
  display.setCursor(xC+100, yC+350);
  display.println("is correctly uploaded");
  display.setCursor(xC+100, yC+400);
  display.print("in your DiPLab Petri!");
  display.display();
}

void state8(){
  //Bacteria spreading (everything is in the loop)
  accX = 0;
  accY = 0;
  Serial.println("State 8 activated!");
}

void state9(){
  //Antibiotic selection
  if (arrAb[0]+arrAb[1]+arrAb[2] == 3){
    //All antibiotics are displayed
    display.clearDisplay();
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    display.setCursor(xC+100, yC+250);
    display.print("AB 1: ");
    display.println(inputMessage3);
    display.setCursor(xC+100, yC+300);
    display.print("AB 2: ");
    display.println(inputMessage4);
    display.setCursor(xC+100, yC+350);
    display.print("AB 3: ");
    display.println(inputMessage5);
    display.setCursor(xC+100, yC+400);
    display.print("are correctly uploaded");
    display.setCursor(xC+100, yC+450);
    display.println("in your DiPLab Petri!");
    display.display();
  } else if(arrAb[0]+arrAb[1]+arrAb[2] == 2){
      display.clearDisplay();
      display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
      display.setCursor(xC+100, yC+350);
      display.println("You need to select 1 more AB");
      display.partialUpdate();
  } else if (arrAb[0]+arrAb[1]+arrAb[2] == 1){
      display.clearDisplay();
      display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
      display.setCursor(xC+100, yC+350);
      display.println("You need to select 2 more ABs");
      display.partialUpdate();
  } else if (arrAb[0]+arrAb[1]+arrAb[2] == 0){
      display.clearDisplay();
      display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
      display.setCursor(xC+100, yC+350);
      display.println("You need to select 3 ABs");
      display.partialUpdate();
  }
}

void state10(){
  //Insert of Antibiotic (Shake device!)
  accX = 0;
  accY = 0;
  Serial.println("State 10 activated!");
}

void state11(){
  //Control center (History/Microscope mode)
  return;
}

void state12(){
  //Save the experience
  return;
}