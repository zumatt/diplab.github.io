/*
    -----------------------------------------------------------
                          WEB EVENT
    -----------------------------------------------------------
*/

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) { // event structure

  switch (type) {
    /*
    case WStype_DISCONNECTED: // client disconnected
      Serial.println("Client disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("Client connected");
      break;*/
    case WStype_TEXT:
      // try to decipher the JSON string received
      StaticJsonDocument<200> doc;                    // create a JSON container
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else {
        // JSON string was received correctly, so information can be retrieved:
        String temp_name, temp_bacteria, temp_ab1, temp_ab2, temp_ab3, temp_controlCenter;

        const int   d_state = doc["state"];
        const char* d_name = doc["name"];
        const int   d_classcode = doc["classcode"];
        const char* d_bacteria = doc["bacteria"];
        const char* d_ab1 = doc["ab1"];
        const char* d_ab2 = doc["ab2"];
        const char* d_ab3 = doc["ab3"];
        const char* d_controlCenter = doc["controlCenter"];
        const int   d_test = doc["test"];
        const int   d_readingAb = doc["readingAb"];

        temp_name = d_name;
        temp_bacteria = d_bacteria;
        temp_ab1 = d_ab1;
        temp_ab2 = d_ab2;
        temp_ab3 = d_ab3;
        temp_controlCenter = d_controlCenter;

        j_state = d_state;
        if (temp_bacteria.length() != 0){j_bacteria = d_bacteria;}
        if (temp_ab1.length() != 0){j_ab1 = d_ab1;}
        if (temp_ab2.length() != 0){j_ab2 = d_ab2;}
        if (temp_ab3.length() != 0){j_ab3 = d_ab3;}
        if (temp_name.length() != 0){j_name = d_name;}
        if (d_classcode != 0){j_classcode = d_classcode;}
        j_readingAB = d_readingAb;
        j_test = d_test;
        if (temp_controlCenter.length() != 0){j_controlCenter = d_controlCenter;}
        
        expState(d_state);

        Serial.println("----------------------------------");
        Serial.println("");
        Serial.println("Info received from webPage:");
        Serial.println("");
        Serial.print("  - State: "); Serial.println(j_state);
        Serial.print("  - Name: "); Serial.println(j_name);
        Serial.print("  - Classcode: "); Serial.println(j_classcode);
        Serial.print("  - Bacteria: "); Serial.println(j_bacteria);
        Serial.print("  - Antibiotic 1 : "); Serial.println(j_ab1);
        Serial.print("  - Antibiotic 2 : "); Serial.println(j_ab2);
        Serial.print("  - Antibiotic 3 : "); Serial.println(j_ab3);
        Serial.print("  - Control center : "); Serial.println(j_controlCenter);
        Serial.print("  - Test : "); Serial.println(j_controlCenter);
        Serial.print("  - Reading AB : "); Serial.println(j_readingAB);
        Serial.println("");
        Serial.println("----------------------------------");
      }
      Serial.println("");
      break;
  }
  Serial.println(" ");

}