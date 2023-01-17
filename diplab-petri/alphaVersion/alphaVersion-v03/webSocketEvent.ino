void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) { // event structure

  switch (type) {
    case WStype_DISCONNECTED: // client disconnected
      Serial.println("Client disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("Client connected");
      break;
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
        const char* g_brand = doc["brand"];
        const char* g_type = doc["type"];
        const int g_year = doc["year"];
        const char* g_color = doc["color"];
        const char* g_location = doc["location"];
        Serial.println("Received guitar info from user");
        Serial.println("Brand: " + String(g_brand));
        Serial.println("Type: " + String(g_type));
        Serial.println("Year: " + String(g_year));
        Serial.println("Color: " + String(g_color));
        Serial.println("Location: " + String(g_location));
      }
      Serial.println("");
      break;
  }
  Serial.println(" ");

}