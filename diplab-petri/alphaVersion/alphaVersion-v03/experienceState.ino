/*
    -----------------------------------------------------------
                      EXPERIENCE CONTROLLER
    -----------------------------------------------------------
*/
void expState(int sNum){
  if(sNum == 0){
    state0();
  } else if(sNum == 1){
    state1();
  } else if(sNum == 6){
    state6();
  } else if(sNum == 7){
    state7();
  } else if(sNum == 8){
    state8();
  } else if(sNum == 9){
    state9();
  } else if(sNum == 10){
    state10();
  } else if(sNum == 11){
    state11();
  } else if(sNum == 12){
    state12();
  }
}

/*
    -----------------------------------------------------------
                          STATE FUNCTIONS
    -----------------------------------------------------------
*/

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
  display.println("You can start the experience!");
  display.display();
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
  updateResistanceValue();

  //Reset value of j_controlCenter & j_microscopeAB
  j_microscopeAB = 0;
  j_controlCenter = "";

  //Start experience: enter Name and class code
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+250);
  display.println("Your name:");
  display.setCursor(xC+100, yC+300);
  Serial.print("Name from the web : ");
  Serial.println(j_name);
  display.println(j_name);
  display.setCursor(xC+100, yC+400);
  display.print("Your class code:");
  display.setCursor(xC+100, yC+450);
  display.print("#");
  Serial.print("Class code from the web : ");
  Serial.println(j_classcode);
  display.println(j_classcode);
  display.display();
}

void state7(){
  //Bacteria selection
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+300);
  display.println(j_bacteria);
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
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.display();
  readyToSpread = 1;
  Serial.println("State 8 activated!");
}

void state9(){
  //Reset controller
  readyToSpread = 0;

  //Antibiotic selection
  if (arrAb[0]+arrAb[1]+arrAb[2] == 3){
    //All antibiotics are displayed
    display.clearDisplay();
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    display.setCursor(xC+100, yC+250);
    display.print("AB 1: ");
    display.println(j_ab1);
    display.setCursor(xC+100, yC+300);
    display.print("AB 2: ");
    display.println(j_ab2);
    display.setCursor(xC+100, yC+350);
    display.print("AB 3: ");
    display.println(j_ab3);
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
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  baseLayer();
  display.display();
  Serial.println("State 10 activated!");
}

void state11(){
  //Control center (History/Microscope mode)
  if(j_test == 1){
    //The next three lines are only for test!
    //To start test pass the query <ESP_IP>/string?test=1
    ab1_resistance = 2;
    ab2_resistance = 4;
    ab3_resistance = 8;
    updateResistanceValue();
  }
  if(j_controlCenter == "history"){
    Serial.println("We are in history mode!");
    historyMode();
  }else if(j_controlCenter == "microscope"){
    Serial.println("We are in microscope mode!");
    microscopeMode();
  } else{Serial.println("Error in state11");}
}

void state12(){
  //Save the experience
  baseLayer();

      display.fillCircle(ab1_x, ab1_y, abDiameter*ab1_resistance, 0);
      display.fillCircle(ab2_x, ab2_y,  abDiameter*ab2_resistance, 0);
      display.fillCircle(ab3_x, ab3_y,  abDiameter*ab3_resistance, 0);
    display.fillCircle(ab1_x, ab1_y, abDiameter, 1);
    display.fillCircle(ab2_x, ab2_y, abDiameter, 1);
    display.fillCircle(ab3_x, ab3_y, abDiameter, 1);
    display.display();

    String jsonString = "";
    StaticJsonDocument<200> doc;                      // create a JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["ab1_x"] = ab1_x;                          // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
    object["ab1_y"] = ab1_y;
    object["ab2_x"] = ab2_x;
    object["ab2_y"] = ab2_y;
    object["ab3_x"] = ab3_x;
    object["ab3_y"] = ab3_y;
    object["ab1_resistance"] = ab1_resistance;
    object["ab2_resistance"] = ab2_resistance;
    object["ab3_resistance"] = ab3_resistance;
    serializeJson(doc, jsonString);                   // convert JSON object to string
    Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
    webSocket.broadcastTXT(jsonString);               // send JSON string to clients
}