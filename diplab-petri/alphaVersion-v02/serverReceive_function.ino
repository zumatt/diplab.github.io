void serverReceive(){
  server.on("/string", HTTP_GET, [] (AsyncWebServerRequest *request) {

    // GET input1 value on <ESP_IP>/string?state
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
    }
    else {
      //inputMessage1 = "No message sent";
    }
    // GET input2 value on <ESP_IP>/string?bacteria
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    }
    else {
      //inputMessage2 = "No message sent";
    }
    // GET input3 value on <ESP_IP>/string?ab1
    if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      if(arrAb[0] == 0){arrAb[0] = 1;}
    }
    else {
      //inputMessage3 = "No message sent";
      if(arrAb[0] == 1){arrAb[0] = 0;}
    }
    // GET input4 value on <ESP_IP>/string?ab2
    if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage4 = request->getParam(PARAM_INPUT_4)->value();
      if(arrAb[1] == 0){arrAb[1] = 1;}
    }
    else {
      //inputMessage4 = "No message sent";
      if(arrAb[1] == 1){arrAb[1] = 0;}
    }
    // GET input5 value on <ESP_IP>/string?ab3
    if (request->hasParam(PARAM_INPUT_5)) {
      inputMessage5 = request->getParam(PARAM_INPUT_5)->value();
      if(arrAb[2] == 0){arrAb[2] = 1;}
    }
    else {
      //inputMessage5 = "No message sent";
      if(arrAb[2] == 1){arrAb[2] = 0;}
    }
    // GET input6 value on <ESP_IP>/string?name
    if (request->hasParam(PARAM_INPUT_6)) {
      inputMessage6 = request->getParam(PARAM_INPUT_6)->value();
    }
    else {
      //inputMessage6 = "No message sent";
    }
     // GET input7 value on <ESP_IP>/string?classcode
    if (request->hasParam(PARAM_INPUT_7)) {
      inputMessage7 = request->getParam(PARAM_INPUT_7)->value();
    }
    else {
      //inputMessage7 = "No message sent";
    }
     // GET input8 value on <ESP_IP>/string?microscopeAb
    if (request->hasParam(PARAM_INPUT_8)) {
      inputMessage8 = request->getParam(PARAM_INPUT_8)->value();
    }
    else {
      //inputMessage8 = "No message sent";
    }
     // GET input9 value on <ESP_IP>/string?test
    if (request->hasParam(PARAM_INPUT_9)) {
      inputMessage9 = request->getParam(PARAM_INPUT_9)->value();
    }
    else {
      inputMessage9 = "2";
    }
     // GET input9 value on <ESP_IP>/string?controlCenter
    if (request->hasParam(PARAM_INPUT_10)) {
      inputMessage10 = request->getParam(PARAM_INPUT_10)->value();
    }
    else {
      //inputMessage10 = "No message sent";
    }

    //Check state
    if(inputMessage1 == "0")      {request->send(SPIFFS, "/state0.html", "text/html");state0();}
      else if(inputMessage1 == "1") {request->send(SPIFFS, "/state1.html", "text/html");state1();}
      else if(inputMessage1 == "2") {request->send(SPIFFS, "/state2.html", "text/html");state2(0);}
      else if(inputMessage1 == "2.1") {request->send(SPIFFS, "/state2-1.html", "text/html");state2(1);}
      else if(inputMessage1 == "3") {request->send(SPIFFS, "/state3.html", "text/html");state3(0);}
      else if(inputMessage1 == "3.1") {request->send(SPIFFS, "/state3-1.html", "text/html");state3(1);}
      else if(inputMessage1 == "4") {request->send(SPIFFS, "/state4.html", "text/html");state4();}
      else if(inputMessage1 == "5") {request->send(SPIFFS, "/state5.html", "text/html");state5();}
      else if(inputMessage1 == "6") {request->send(SPIFFS, "/state6.html", "text/html");state6();}
      else if(inputMessage1 == "7") {request->send(SPIFFS, "/state7.html", "text/html");state7();}
      else if(inputMessage1 == "8") {request->send(SPIFFS, "/state8.html", "text/html");state8();}
      else if(inputMessage1 == "9") {request->send(SPIFFS, "/state9.html", "text/html");state9();}
      else if(inputMessage1 == "10"){request->send(SPIFFS, "/state10.html", "text/html");state10();}
      else if(inputMessage1 == "11"){request->send(SPIFFS, "/state11.html", "text/html");state11();}
      else if(inputMessage1 == "12"){request->send(SPIFFS, "/state12.html", "text/html");state12();}

    //Print on console the input messages
    Serial.print("Test: ");
    if(inputMessage9 == "0"){ Serial.println("No");}else if(inputMessage9 == "1"){Serial.println("Yes");}else if(inputMessage9 == "2"){Serial.println("Not received - could be no.");}else{Serial.println("Error!");}
    Serial.println("----------------------------------");
    Serial.println("Query from the browser: ");
    Serial.print(" - State: ");
    Serial.println(inputMessage1);
    Serial.print(" - Bateria: ");
    Serial.println(inputMessage2);
    Serial.print(" - Antibiotic n°1: ");
    Serial.println(inputMessage3);
    Serial.print(" - Antibiotic n°2: ");
    Serial.println(inputMessage4);
    Serial.print(" - Antibiotic n°3: ");
    Serial.println(inputMessage5);
    Serial.print(" - Name: ");
    Serial.println(inputMessage6);
    Serial.print(" - Class Code: ");
    Serial.println(inputMessage7);
    Serial.print(" - Microscope AB: ");
    Serial.println(inputMessage8);
    Serial.print(" - Control center modality: ");
    Serial.println(inputMessage10);
    Serial.println("----------------------------------");
    Serial.println();
  });
}
