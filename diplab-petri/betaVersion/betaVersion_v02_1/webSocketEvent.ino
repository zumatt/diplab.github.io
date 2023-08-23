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

        j_state = d_state;
        if (d_name != ""){j_bacteria = d_bacteria;}
        if (d_ab1 != ""){j_ab1 = d_ab1;}
        if (d_ab2 != ""){j_ab2 = d_ab2;}
        if (d_ab3 != ""){j_ab3 = d_ab3;}
        if (d_name != ""){j_name = d_name;}
        if (d_classcode != 0){j_classcode = d_classcode;}
        j_readingAB = d_readingAb;
        j_test = d_test;
        if (d_controlCenter != ""){j_controlCenter = d_controlCenter;}
        
        expState(d_state);

        Serial.println("----------------------------------");
        Serial.println("");
        Serial.println("Info received from webPage:");
        Serial.println("");
        Serial.println("  - State: " + String(j_state));
        Serial.println("  - Name: " + String(j_name));
        Serial.println("  - Classcode: " + String(j_classcode));
        Serial.println("  - Bacteria: " + String(j_bacteria));
        Serial.println("  - Antibiotic 1 : " + String(j_ab1));
        Serial.println("  - Antibiotic 2 : " + String(j_ab2));
        Serial.println("  - Antibiotic 3 : " + String(j_ab3));
        Serial.println("  - Control center : " + String(j_controlCenter));
        Serial.println("  - Test : " + String(j_controlCenter));
        Serial.println("  - Reading AB : " + String(j_readingAB));
        Serial.println("");
        Serial.println("----------------------------------");
      }
      Serial.println("");
      break;
  }
  Serial.println(" ");

}