/*
    -----------------------------------------------------------
              DEVICE READY TO COLLECT TOUCHSCREEN INFO
    -----------------------------------------------------------
*/

void abPlacing(){
  // Check if there is any touch detected
  if (display.tsAvailable()){
    display.setTextColor(BLACK, WHITE);
    
    if (antibioticCreated < 3){
      uint8_t n;
      uint16_t x[2], y[2];
      // See how many fingers are detected (max 2) and copy x and y position of each finger on touchscreen
      n = display.tsGetData(x, y);
      if (n != 0){
        Serial.println("Touch on screen detected!");
        if (antibioticCreated == 1){
          differenceCoordX1 = ab1_x - x[0];
          differenceCoordY1 = ab1_y - y[0];
        } else if (antibioticCreated == 2){
          differenceCoordX2 = ab2_x - x[0];
          differenceCoordY2 = ab2_y - y[0];
          differenceCoordX1 = ab1_x - x[0];
          differenceCoordY1 = ab1_y - y[0];
        }            
        
        if ((!(differenceCoordX1 >= -tresholdAntibiotic && differenceCoordX1 <= tresholdAntibiotic) ||
              !(differenceCoordY1 >= -tresholdAntibiotic && differenceCoordY1 <= tresholdAntibiotic)) ||
              (differenceCoordX1 == 0 && differenceCoordY1 == 0)) {
                
                if ((!(differenceCoordX2 >= -tresholdAntibiotic && differenceCoordX2 <= tresholdAntibiotic) ||
                      !(differenceCoordY2 >= -tresholdAntibiotic && differenceCoordY2 <= tresholdAntibiotic)) ||
                      (differenceCoordX2 == 0 && differenceCoordY2 == 0)) {
                        display.fillCircle(x[0], y[0], abDiameter, WHITE);
                        display.setCursor(x[0] - abDiameter/2, y[0] + abDiameter/3);
                        if (antibioticCreated == 0) {display.println("1");} else if (antibioticCreated == 1){display.println("2");} else if (antibioticCreated == 2){display.println("3");}
                        display.partialUpdate();
                        if (antibioticCreated == 0){
                          ab1_x = x[0];
                          ab1_y = y[0];
                        } else if (antibioticCreated == 1){
                          ab2_x = x[0];
                          ab2_y = y[0];
                        } else if (antibioticCreated == 2){
                          ab3_x = x[0];
                          ab3_y = y[0];
                        }
                        antibioticCreated += 1;
                        Serial.println(antibioticCreated);
                } else {
                  Serial.println("Error generating AB - Code 2");
                  errorCheck(x[0], y[0]);}
        } else {
                Serial.println("Error generating AB - Code 1");
                errorCheck(x[0], y[0]);}
      }
    } else if (antibioticCreated == 3){
      String jsonString = "";
      StaticJsonDocument<200> doc;                      // create a JSON container
      JsonObject object = doc.to<JsonObject>();         // create a JSON Object

      object["ab_placed"] = 1;                          // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else

      serializeJson(doc, jsonString);                   // convert JSON object to string
      Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
      webSocket.broadcastTXT(jsonString);               // send JSON string to clients
      
      Serial.println("Antibiotic placed correctly");
      Serial.println(antibioticCreated);
    }
  }
}

void errorCheck(float xCoord, float yCoord){
  Serial.print("Previous coordinate X: ");
  Serial.println(ab1_x);
  Serial.print("Previous coordinate Y: ");
  Serial.println(ab1_y);
  Serial.print("Coordinate X: ");
  Serial.println(xCoord);
  Serial.print("Coordinate Y: ");
  Serial.println(yCoord);
  Serial.print("Calc X 1: ");
  Serial.println(differenceCoordX1);
  Serial.print("Calc Y 1: ");
  Serial.println(differenceCoordY1);
  Serial.print("Calc X 2: ");
  Serial.println(differenceCoordX2);
  Serial.print("Calc Y 2: ");
  Serial.println(differenceCoordY2);
}
