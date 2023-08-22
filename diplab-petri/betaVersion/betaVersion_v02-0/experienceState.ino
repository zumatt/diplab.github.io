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
  //display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
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

  //Reset value of j_controlCenter & j_readingAB
  j_readingAB = 0;
  j_controlCenter = "";

  //Start experience: enter Name and class code
  display.clearDisplay();
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
  display.setCursor(xC+100, yC+300);
  display.println(j_bacteria);
  display.setCursor(xC+100, yC+350);
  display.println("is correctly uploaded");
  display.setCursor(xC+100, yC+400);
  display.print("in your DiPLab Petri!");
  display.display();

  //Create all the dots for the display of bacteria growing
    for(int i=0; i<500; i++){
      bacteriaDotsX[i] = random(xC, xC2);
      bacteriaDotsX[i] = random(yC, yC2);
    }
}

void state8(){
  //Bacteria spreading (everything is in the loop)
  accX = 0;
  accY = 0;
  display.clearDisplay();
  display.display();
  readyToSpread = 1;
  Serial.println("State 8 activated!");
}

void state9(){
  //Reset controller
  readyToSpread = 0;

  //Antibiotic selection
    display.clearDisplay();
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
  //Antibiotic resistance calculation (for now is just a test)
    ab1_resistance = 2;
    ab2_resistance = 3.5;
    ab3_resistance = 8;
}

void state10(){
  //Insert of Antibiotic (Touchscreen)
  accX = 0;
  accY = 0;
  display.clearDisplay();
    display.fillScreen(BLACK);
  display.display();
  Serial.println("State 10 activated!");
}

void state11(){
  //Control center (History/Reading mode)
  if(j_controlCenter == "history"){
    Serial.println("We are in history mode!");
    historyHours = 0;
    historyMode();
  }else if(j_controlCenter == "reading"){
    Serial.println("We are in reading mode!");
    readingMode();
  } else{Serial.println("Error in state11");}
}

void state12(){
  //Save the experience
  display.clearDisplay();
  display.fillScreen(BLACK);

      display.fillCircle(ab1_x, ab1_y, abDiameter*ab1_resistance, 0);
      display.fillCircle(ab2_x, ab2_y,  abDiameter*ab2_resistance, 0);
      display.fillCircle(ab3_x, ab3_y,  abDiameter*ab3_resistance, 0);
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