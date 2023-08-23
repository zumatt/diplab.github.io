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
  graphicLanding();
}

void state1() {
  graphicConnected();
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
  //ab1_resistance = 0;
  //ab2_resistance = 0;
  //ab3_resistance = 0;

  //Reset value of j_controlCenter & j_readingAB
  j_readingAB = 0;
  j_controlCenter = "";

  graphicNameClasscodeReceived();
  sleep(2.5);
  graphicWaitingBact();
}

void state7(){
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
  graphicWaitingAb();
}

void state10(){
  //Insert of Antibiotic (Touchscreen)
  accX = 0;
  accY = 0;
  display.clearDisplay();
  display.setTextSize(3);
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
    historyCondition();
  }else if(j_controlCenter == "reading"){
    Serial.println("We are in reading mode!");
    readingMode();
  } else{Serial.println("Error in state11");}
}

void state12(){
  //Save the experience
    historyBase(500);

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