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
    } else if(j_controlCenter == "microscope"){  
      if(lastState_fwd == LOW && currentState_fwd == HIGH){
        if (counter < 1){counter ++;}
        microscopeCondition();
      }   
      if(lastState_bck == LOW && currentState_bck == HIGH){
        if (counter != 0){counter --;}
        microscopeCondition();
      }
    }

    lastState_fwd = currentState_fwd;
    lastState_bck = currentState_bck;
    }
