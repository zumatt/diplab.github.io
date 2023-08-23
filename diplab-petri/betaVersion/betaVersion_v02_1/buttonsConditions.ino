/*
    -----------------------------------------------------------
                          BTN CHECK
    -----------------------------------------------------------
*/

void buttonsCondition(){
if(j_controlCenter == "history"){  
      if(lastState_fwd == LOW && currentState_fwd == HIGH){
        if (counter < 3){counter ++;}
        historyCondition();
      }   
      if(lastState_bck == LOW && currentState_bck == HIGH){
        if (counter != 0){counter --;}
        historyCondition();
      }
    }

    lastState_fwd = currentState_fwd;
    lastState_bck = currentState_bck;
    }

void buttonTouchScreen(){
    if (display.tsAvailable()){
      uint8_t n;
      uint16_t x[2], y[2];
      // See how many fingers are detected (max 2) and copy x and y position of each finger on touchscreen
      n = display.tsGetData(x, y);
      if (n != 0){
        if(j_controlCenter == "history"){  
          if(x[0] >= xC && x[0] < dispH/2){
            Serial.println("Touchscreen Button Back");

            if (counter != 0){counter --;}
            historyCondition();
          } else if (x[0] >= dispH/2 && x[0] <= xC2){
            Serial.println("Touchscreen Button Forward");
            
            if (counter < 3){counter ++;}
            historyCondition();
          }
        }
      }
  }
}
