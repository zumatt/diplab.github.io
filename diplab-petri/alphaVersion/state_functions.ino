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

void state2(){
  //Introduction spread bacteria
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.display();
  for (int i=1; i<=20; i++){
    display.drawLine(xC, yPos-(i*step), xC2, yPos-(i*step), BLACK);
    display.partialUpdate();
    //delay(200);
    }
  for (int j=1; j<=20; j++){
    display.drawLine(xPos+(j*step), yC, xPos+(j*step), yC2, BLACK);
    display.partialUpdate();
    //delay(200);
    }
}

void state3(){
  //Introduction place antibiotics
    display.clearDisplay();

    //Redraw base layer
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    for(int i=1; i<=20; i++){  
      display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}
    for(int j=1; j<=20; j++){  
      display.drawLine(xC, yC+(j*petriD/10), xC2, yC+(j*petriD/10), BLACK);}
    //END of redraw base layer

    display.fillCircle(random(xC+100, xC2-100), random(xC+100, xC2-100), 25, 1);  //First Antibiotic
    display.fillCircle(random(xC+100, xC2-100), random(xC+100, xC2-100), 25, 1);  //Second Antibiotic
    display.fillCircle(random(xC+100, xC2-100), random(xC+100, xC2-100), 25, 1);  //Third Antibiotic

    display.display();
}

void state4(){
  //Introduction history mode
  historyMode();
  delay(300);
  ::counter = 1;
  historyCondition();
  delay(300);
  ::counter = 2;
  historyCondition();
  delay(300);
  ::counter = 3;
  historyCondition();
  delay(300);
}

void state5(){
  //Introduction microscope mode
  microscopeMode();
  delay(300);
  ::counter = 1;
  microscopeCondition();
  delay(300);
  ::counter = 2;
  microscopeCondition();
  delay(300);
  ::counter = 3;
  microscopeCondition();
  delay(300);
}

void state6(){
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
  return;
}

void state8(){
  //Bacteria spreading
  return;
}

void state9(){
  //Antibiotic selection
  return;
}

void state10(){
  //Insert of Antibiotic (Shake device!)
  return;
}

void state11(){
  //Control center (History/Microscope mode)
  return;
}

void state12(){
  //Save the experience
  return;
}