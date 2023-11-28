/* MIT License 2023 Matteo Subet <hi@zumat.ch>
   
   Release Version 0.1 - DiPLaB
   Spearhead Research Project - https://spearhead-amr.github.io/makeaware/
   
        Coded by Matteo Subet - Last version of 15 November 2023
   
   SUPSI - University of Applied Sciences and Arts of Southern Switzerland
   DACD - Department of Architecture, Construction and Design
   IDe - Design Institute

   Based on Inkplate 6PLUS (ESP32).
   You can find the complete repository here: https://github.com/zumatt/diplab
*/





/*
    -----------------------------------------------------------
                            BOARD CHECK
    -----------------------------------------------------------
*/
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif





/*
    -----------------------------------------------------------
                            LIBRARIES
    -----------------------------------------------------------
*/
#include "Inkplate.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebSrv.h"
#include "SPIFFS.h"
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "Fonts/FreeSansBold9pt7b.h"
    //Font setup for FreeSansBold9pt7b
    const GFXfont *textFont = &FreeSansBold9pt7b;





/*
    -----------------------------------------------------------
                            VARIABLES
    -----------------------------------------------------------
*/
//String to save the Access Point SSID and password
    const char* ssid = "GreenDiPLab";
    const char* password = "FightAMR2023";

//String to save the height and width of the display
    int screenWidth = E_INK_WIDTH;                                              //Width of the display from the library
    int screenHeight = E_INK_HEIGHT;                                            //Height of the display from the library

//Variables to store the petri position and dimension
    int petriDiameter = (screenHeight/2)-10;                                           //Diameter of the petri dish
    
    int yCenter = screenHeight/2 - 150;                                                //Center y of petri dish
    int xCenter = screenWidth/2;                                                       //Center x of petri dish
    int xLeftPetri =  (xCenter)-(petriDiameter);                                       //X position of left side of petri dish
    int yTopPetri =  (yCenter)-(petriDiameter);                                        //Y position of top side of petri dish
    int xRightPetri = (xCenter)+(petriDiameter);                                       //X position of right side of petri dish
    int yBottomPetri = (yCenter)+(petriDiameter);                                      //Y position of bottom side of petri dish

//Variables to store information from the web application
    int    webState;                    //State value from the web page
    String webBacteria;                 //Bacteria name from the web page
    String webAb1;                      //Antibiotic 1 name from the web page
    String webAb2;                      //Antibiotic 2 name from the web page
    String webAb3;                      //Antibiotic 3 name from the web page
    String webOperatorName;             //Operator name from the web page
    int    webPatientCode;              //Patient code from the web page
    int    webReadingAB;                //Check wich antibiotic is being read (useful for state 5: control center)
    int    webReadingTime;              //Hour passed 0h / 8h / 12h / 24h (useful for state 5: control center)
    String webControlCenter;            //Control center function selected from the web page
    String webResetPosAb;               //Check if the button to reset antibiotic position is pressed

//Controller for screen ready to spread bacteria
    bool readyToSpread;

//Variables to store information about bacteria spreading
    int    bacteriaLinesStep = petriDiameter/10;                                //Step of each line of bacteria
    float  bacteriaDotsX[500];                                                  //Array that contains all the X values of the dots created for the bacteria growth
    float  bacteriaDotsY[500];                                                  //Array that contains all the Y values of the dots created for the bacteria growth

    float  bacteriaZone = bacteriaLinesStep/2;                                  //General dimension of the rectangle created to spread bacteria
    int    bacteriaSpreadY[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   //Array of Y axis for checking if a line was drawn
    int    lenBactSpreadY = *(&bacteriaSpreadY + 1) - bacteriaSpreadY;          //Array of Y axis length
    bool   completeBactSpreadY;                                                 //Bool to check if the bacteria spread is completed on X axis
    int    bacteriaSpreadX[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   //Array of X axis for checking if a line was drawn
    int    lenBactSpreadX = *(&bacteriaSpreadX + 1) - bacteriaSpreadX;          //Array of X axis length
    bool   completeBactSpreadX;                                                 //Bool to check if the bacteria spread is completed on Y axis
    bool   arrayCompletedSpreading;                                             //Bool to check if the bacteria spread is completed on both axis
    double accelValueX;                                                         //Variable to store accellerometer data on X axis
    double accelValueY;                                                         //Variable to store accellerometer data on Y axis

//Variables to store information about antibiotics position and dimension
    int    abDiameter = 35;                                                     //Dimension of the AB tablet
    int    tresholdAntibioticPosition = 150;                                    //Threshold between two anbiotics pills
    double abMmToPxMultiplier = 0.35;                                           //Multiplier from pixels to mm for ab resistance

    int    ab1XPos, ab1YPos;                                                    //X and Y position of AB 1
    int    ab2XPos, ab2YPos;                                                    //X and Y position of AB 2
    int    ab3XPos, ab3YPos;                                                    //X and Y position of AB 3
    double ab1Resistance;                                                       //Resistance of AB 1 expressed in mm as reported in the antibiotic sheet
    double ab2Resistance;                                                       //Resistance of AB 2 expressed in mm as reported in the antibiotic sheet
    double ab3Resistance;                                                       //Resistance of AB 3 expressed in mm as reported in the antibiotic sheet
    int    antibioticsCreated = 0;                                              //Numbers of antibiotic placed
    int    oldCoordX1 = 0;                                                      //Variable to store the old X position of AB 1
    int    oldCoordY1 = 0;                                                      //Variable to store the old Y position of AB 1
    int    oldCoordX2 = 0;                                                      //Variable to store the old X position of AB 2
    int    oldCoordY2 = 0;                                                      //Variable to store the old Y position of AB 2
    int    oldCoordX3 = 0;                                                      //Variable to store the old X position of AB 3
    int    oldCoordY3 = 0;                                                      //Variable to store the old Y position of AB 3
    int    ab1WebX, ab1WebY;                                                    //Position of first antibiotic on the web page
    int    ab2WebX, ab2WebY;                                                    //Position of second antibiotic on the web page
    int    ab3WebX, ab3WebY;                                                    //Position of third antibiotic on the web page
    int    ab1ResistanceWeb, ab2ResistanceWeb, ab3ResistanceWeb;                //Store resistance value for each AB to display it on the web page

//Variables to store information about the Access Point
    IPAddress serverIP;                                                         //IP address of the server
    int interval = 1000;                                                        //Interval between each check of the IP address
    unsigned long previousMillis = 0;                                           //Variable to store the previous time





/*
    -----------------------------------------------------------
                    FUNCTIONS TO SETUP THE BOARD
    -----------------------------------------------------------
*/
//Create AsyncWebServer object on port 80
    AsyncWebServer server(80);

//Create webSocket on port 81
    WebSocketsServer webSocket = WebSocketsServer(81);

// Create an object on Inkplate library and also set library into 1 Bit mode (BW)
    Inkplate display(INKPLATE_1BIT);





/*
    -----------------------------------------------------------
                    SETUP WIFI AND SDCARD FUNCTIONS
    -----------------------------------------------------------
*/

//Function to check if the SD card is working properly
void initSPIFFS(){
    //Condition to check if the SD card is working properly
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
}

//Function to start the Wifi connection in Access Point mode
void initWiFi() {
    WiFi.begin();                       //Start the WiFi connection
    WiFi.mode(WIFI_AP);                 //WIFI_AP is equal of Access Point mode
    WiFi.softAP(ssid, password);        //AccessPoint ssid and pss
    Serial.print("IP Address: ");
        serverIP = WiFi.softAPIP();     //Get the IP address of the server
    Serial.println(serverIP);           //Here we print the IP Address of the AccessPoint
}

/*
    -----------------------------------------------------------
                          SETUP FUNCTION
    -----------------------------------------------------------
*/

void setup() {
    //Start the serial communication
        Serial.begin(115200);

    //Begin and clear display
        display.begin();
        display.clearDisplay();
        display.display();

    //Setup accellerometer
        LIS.begin(WIRE,0x19); //IIC init
        delay(100);
        LIS.setFullScaleRange(LIS3DHTR_RANGE_4G);
        LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

    //Display settings
        display.setFont(textFont);
        display.setTextSize(2);
        display.setTextColor(BLACK, WHITE);
        display.setTextWrap(true);

    //Start the WiFi connection
        Serial.println("WiFi init:");
        initWiFi();
        Serial.println("WiFi init END");

    //Start the SPIFFS
        Serial.println("SPIFFS init START");
        initSPIFFS();
        Serial.println("SPIFFS init END");

    //Setup of the server first webpage and connected files (e.g. css, favicon, etc.)
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/index.html", "text/html");
        });
        server.serveStatic("/", SPIFFS, "/");

    //Start the server & the webSocket
        server.begin();
        webSocket.begin();

    //When receiving a message from the webSocket, call the webSocketEvent function
        webSocket.onEvent(webSocketEvent);

    //Start the display and check if it works properly
        if (display.tsInit(true)){
            Serial.println("Touchscreen init ok");}
        else {
            Serial.println("Touchscreen init fail");
            while (true);}

    //Start with the experience
        state0();

    //Create all the dots for the display of bacteria growing
        for(int i=0; i<500; i++){
            bacteriaDotsX[i] = random(xLeftPetri, xRightPetri);
            bacteriaDotsY[i] = random(yTopPetri, yBottomPetri);
        }
}




/*
    -----------------------------------------------------------
                          LOOP FUNCTION
    -----------------------------------------------------------
*/

void loop() {
    //Check if there is a message from the webSocket
        webSocket.loop();
    
    //Check if we are in the state for bacteria spreading
    if(webState == 2){
        //Call drawing function for bacteria
            if(webState == 8 && readyToSpread == 1){
            //Update variables for accellerometer
            accelValueX = LIS.getAccelerationX(); //Get accellerometer X data
            accelValueY = -LIS.getAccelerationY(); //Get accellerometer Y data

            drawingLoop();

                if (arrayCompletedSpreading == true){
                    String jsonString = "";
                    StaticJsonDocument<200> doc;                      // create a JSON container
                    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
                    object["spreadingBacteria"] = 1;                  // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
                    serializeJson(doc, jsonString);                   // convert JSON object to string
                    Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
                    webSocket.broadcastTXT(jsonString);               // send JSON string to clients
                    } else {
                        checkLoopSpreading();
                    }
            } //END IF data from accellerometer
    }

    //Call touchscreen functions for antibiotics placing
    if (webState == 4){
        if(webResetPosAb != "Reset"){
            abPlacing();   
        } else {
            state4();
            webResetPosAb = "";
        }
    }
}




/*
    -----------------------------------------------------------
             RECEIVING A MESSAGE FROM THE WEB PAGE
    -----------------------------------------------------------
*/

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) { // event structure

  switch (type) {
    case WStype_TEXT:
      // try to decipher the JSON string received
        StaticJsonDocument<200> doc;                                      // create a JSON container
        DeserializationError error = deserializeJson(doc, payload);       // deserialize the JSON document
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());                                  // print the error if there is one  
            return;
        }
        else {
            // JSON string was received correctly, so information can be retrieved:
                //Create temporary variables to store the information received from the web page
                String tempOperatorName, tempBacteria, tempAb1, tempAb2, tempAb3, tempControlCenter, tempResetPosAb;

            //Store the information received from the web page        
                const int   receivedState         = doc["state"];
                const char* receivedBacteria      = doc["bacteria"];
                const char* receivedAb1           = doc["ab1"];
                const char* receivedAb2           = doc["ab2"];
                const char* receivedAb3           = doc["ab3"];
                const char* receivedResetPosAb    = doc["resetAbPos"];
                const char* receivedOperatorName  = doc["name"];
                const int   receivedPatientCode   = doc["patientcode"];
                const int   receivedReadingAb     = doc["readingAb"];
                const int   receivedReadingBact   = doc["readingBacteria"];
                const int   receivedReadingTime   = doc["readingTime"];
                const char* receivedControlCenter = doc["controlCenter"];

            //Store the information received from the web page in the temporary variables        
                tempOperatorName    = receivedOperatorName;
                tempBacteria        = receivedBacteria;
                tempAb1             = receivedAb1;
                tempAb2             = receivedAb2;
                tempAb3             = receivedAb3;
                tempResetPosAb      = receivedResetPosAb;
                tempControlCenter   = receivedControlCenter;

            //Store the information received from the web page in the global variables
                                                            webState            = receivedState;
                if (tempOperatorName.length() != 0){        webOperatorName     = receivedOperatorName; }
                if (tempBacteria.length() != 0){            webBacteria         = receivedBacteria;     }
                if (tempAb1.length() != 0){                 webAb1              = receivedAb1;          }
                if (tempAb2.length() != 0){                 webAb2              = receivedAb2;          }
                if (tempAb3.length() != 0){                 webAb3              = receivedAb3;          }
                if (receivedPatientCode != 0){              webPatientCode      = receivedPatientCode;  }
                                                            webReadingAB        = receivedReadingAb;
                                                            webReadingBact      = receivedReadingBact;
                                                            webReadingTime      = receivedReadingTime;
                if (tempControlCenter.length() != 0){       webControlCenter    = receivedControlCenter;}
                if (tempResetPosAb.length() != 0){          webResetPosAb       = receivedResetPosAb;   }
            
            //Call function related to the state received from the web page
                expState(receivedState);

            //Print in console the information saved on the Petri from the web page
                Serial.println("----------------------------------");
                Serial.println("");
                Serial.println("Info received from webPage:");
                Serial.println("");
                Serial.print("  - State: ");
                    Serial.println(webState);
                Serial.print("  - Name: ");
                    Serial.println(webOperatorName);
                Serial.print("  - Classcode: ");
                    Serial.println(webPatientCode);
                Serial.print("  - Bacteria: ");
                    Serial.println(webBacteria);
                Serial.print("  - Antibiotic 1 : ");
                    Serial.println(webAb1);
                Serial.print("  - Antibiotic 2 : ");
                    Serial.println(webAb2);
                Serial.print("  - Antibiotic 3 : ");
                    Serial.println(webAb3);
                Serial.print("  - Reset position called : ");
                    Serial.println(receivedResetPosAb);
                Serial.print("  - Control center : ");
                    Serial.println(webControlCenter);
                Serial.print("    - Reading AB : ");
                    Serial.println(webReadingAB);
                Serial.print("    - Reading Bact : ");
                    Serial.println(webReadingBact);
                Serial.print("    - Reading time : ");
                    Serial.println(webReadingTime);
                Serial.println("");
                Serial.println("----------------------------------");
        }
        break;
    }
}




/*
    -----------------------------------------------------------
                      EXPERIENCE CONTROLLER
    -----------------------------------------------------------
*/
void expState(int stateVariable){
    //Call function related to the state received from the web page
        if(stateVariable == 0){
            state0();                       //This is the state where the user start the journey; The trigger to move on is the insert of patient code into the phone;
        } else if(stateVariable == 1){
            state1();                       //This is the state where the user connected correctly showing the patient code and the trigger is the bacteria confirmation;
        } else if(stateVariable == 2){
            state2();                       //This is the state where the user need to spread the bacteria; Trigger is the screen entirely covered with bacteria;
        } else if(stateVariable == 3){
            state3();                       //This is the state where the user need to select the antibiotics; Trigger is the third antibiotic selected;
        } else if(stateVariable == 4){
            state4();                       //This is the state where the user insert the three antibiotics with touchscreen. Trigger is the confirmation or reset position;
        } else if(stateVariable == 5){
            state5();                       //This is the state where the user is in control center;
        } else if(stateVariable == 6){
            state6();                       //This is the state where the user finish the experience, the device is showing the last petri dish. Trigger is on the phone to continue the experience (state 2) or end it.
        } else {
            //Return an error message
                Serial.println("Error: state not found");
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
    display.setCursor(xCenter+90, yCenter+200);
    display.setTextSize(2);
    display.println("Welcome to DiPLab");
    display.setCursor(xCenter+90, yCenter+260);
    display.setTextSize(1);
    display.println("the only digital laboratory experience that");
    display.setCursor(xCenter+90, yCenter+290);
    display.println("disseminates the issue of Antimicrobial");
    display.setCursor(xCenter+90, yCenter+320);
    display.println("Resistance using an interactive tool.");
    display.setCursor(xCenter+90, yCenter+390);
    display.print("connect to ");
    display.print(::ssid);
    display.println(" WiFi with the password: ");
    display.setCursor(xCenter+90, yCenter+420);
    display.println(::password);
    display.setCursor(xCenter+90, yCenter+450);
    display.println("open the web browser and go to");
    display.setCursor(xCenter+90, yCenter+480);
    display.print("http://");
    display.print(::serverIP);
    display.println('/');
    
    for (int i=0; i<5; i++){
      display.fillCircle(xCenter+640+(i*30), yCenter + 420, 10, BLACK);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xCenter+610+(i*30), yCenter + 450, 10, BLACK);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xCenter+550+(i*30), yCenter + 480, 10, BLACK);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xCenter+490+(i*30), yCenter + 510, 10, BLACK);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xCenter+340+(i*30), yCenter + 540, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xCenter+280+(i*30), yCenter + 570, 10, BLACK);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xCenter+220+(i*30), yCenter + 600, 10, BLACK);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xCenter+100+(i*30), yCenter + 630, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xCenter+130+(i*30), yCenter + 660, 10, BLACK);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xCenter+160+(i*30), yCenter + 690, 10, BLACK);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xCenter+190+(i*30), yCenter + 720, 10, BLACK);   
    }
    display.setTextSize(2);
    display.display();
}

void state1() {
    display.clearDisplay();
    display.setCursor(xCenter+90, yCenter+200);
    display.setTextSize(2);
    display.println("Your DiPLab is");
    display.setCursor(xCenter+90, yCenter+230);
    display.println("connected!");
    display.setCursor(xCenter+90, yCenter+310);
    display.setTextSize(1);
    display.print("You logged in as Lab ");
    display.print(::webOperatorName);
    display.println("!");
    display.setCursor(xCenter+90, yCenter+360);
    display.print("the patient code inserted is #");
    display.print(::webPatientCode);
    display.partialUpdate();
    
    for (int i=0; i<5; i++){
      display.fillCircle(xCenter+640+(i*30), yCenter + 420, 10, BLACK);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xCenter+610+(i*30), yCenter + 450, 10, BLACK);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xCenter+550+(i*30), yCenter + 480, 10, BLACK);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xCenter+490+(i*30), yCenter + 510, 10, BLACK);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xCenter+340+(i*30), yCenter + 540, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xCenter+280+(i*30), yCenter + 570, 10, BLACK);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xCenter+220+(i*30), yCenter + 600, 10, BLACK);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xCenter+100+(i*30), yCenter + 630, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xCenter+130+(i*30), yCenter + 660, 10, BLACK);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xCenter+160+(i*30), yCenter + 690, 10, BLACK);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xCenter+190+(i*30), yCenter + 720, 10, BLACK);   
    }
    display.setTextSize(2);
    display.display();
}

void state2(){
  //Reset AB position for later
    ab1XPos = 0;
    ab1YPos = 0;
    ab2XPos = 0;
    ab2YPos = 0;
    ab3XPos = 0;
    ab3YPos = 0;
    oldCoordX1 = 0;
    oldCoordY1 = 0;
    oldCoordX2 = 0;
    oldCoordY2 = 0;
    oldCoordX3 = 0;
    oldCoordY3 = 0;
    ab1WebX = 0;
    ab1WebY = 0;
    ab2WebX = 0;
    ab2WebY = 0;
    ab3WebX = 0;
    ab3WebY = 0;

  //Reset resistance value for later
    ab1Resistance = 0;
    ab2Resistance = 0;
    ab3Resistance = 0;
    ab1ResistanceWeb = 0;
    ab2ResistanceWeb = 0;
    ab3ResistanceWeb = 0;

  //Reset value of j_controlCenter & webReadingAB
    webReadingAB = 0;
    webControlCenter = "";

  //Bacteria spreading (everything is in the loop)
    accelValueX = 0;
    accelValueY = 0;
    display.clearDisplay();
    display.display();
    readyToSpread = 1;
}

void state3(){
    //Reset controller
        readyToSpread = 0;
        for(int i=0; i<lenBactSpreadY; i++){bacteriaSpreadY[i] = 0;};
        for(int i=0; i<lenBactSpreadX; i++){bacteriaSpreadX[i] = 0;};
        Serial.print("Bacteria Array");
          for(int i=0; i<lenBactSpreadY; i++){Serial.print(" - ");Serial.print(bacteriaSpreadY[i]);};
        Serial.println(" - END");
        completeBactSpreadY = false;
        completeBactSpreadX = false;
        arrayCompletedSpreading = false;

    //Graphic for waiting Antibiotic
        display.clearDisplay();
        display.fillScreen(BLACK);
        display.setTextColor(WHITE, BLACK);
        display.setCursor(xCenter+90, yCenter+200);
        display.setTextSize(2);
        display.println("Waiting for antibiotic");
        display.setTextSize(1);
        display.setCursor(xCenter+90, yCenter+360);
        display.println("select three antibiotics with your phone");
        
        for (int i=0; i<5; i++){
        display.fillCircle(xCenter+640+(i*30), yCenter + 420, 10, WHITE);   
        }
        for (int i=0; i<8; i++){
        display.fillCircle(xCenter+610+(i*30), yCenter + 450, 10, WHITE);   
        }
        for (int i=0; i<10; i++){
        display.fillCircle(xCenter+550+(i*30), yCenter + 480, 10, WHITE);   
        }
        for (int i=0; i<12; i++){
        display.fillCircle(xCenter+490+(i*30), yCenter + 510, 10, WHITE);   
        }
        for (int i=0; i<15; i++){
        display.fillCircle(xCenter+340+(i*30), yCenter + 540, 10, WHITE);   
        }
        for (int i=0; i<18; i++){
        display.fillCircle(xCenter+280+(i*30), yCenter + 570, 10, WHITE);   
        }
        for (int i=0; i<21; i++){
        display.fillCircle(xCenter+220+(i*30), yCenter + 600, 10, WHITE);   
        }
        for (int i=0; i<19; i++){
        display.fillCircle(xCenter+100+(i*30), yCenter + 630, 10, WHITE);   
        }
        for (int i=0; i<18; i++){
        display.fillCircle(xCenter+130+(i*30), yCenter + 660, 10, WHITE);   
        }
        for (int i=0; i<17; i++){
        display.fillCircle(xCenter+160+(i*30), yCenter + 690, 10, WHITE);   
        }
        for (int i=0; i<11; i++){
        display.fillCircle(xCenter+190+(i*30), yCenter + 720, 10, WHITE);   
        }
        display.setTextSize(2);
        display.display();
}

void state4(){
    //Clear display in black for inserting antibiotics
        display.clearDisplay();
        display.fillScreen(BLACK);
        display.setTextColor(WHITE, BLACK);
        display.setTextSize(3);
        display.display();
}

void state5(){
  //Control center (History/Reading mode)
    ab1WebX = 100*ab1XPos/screenWidth;
    ab1WebY = 100*ab1YPos/screenHeight;
    ab2WebX = 100*ab2XPos/screenWidth;
    ab2WebY = 100*ab2YPos/screenHeight;
    ab3WebX = 100*ab3XPos/screenWidth;
    ab3WebY = 100*ab3YPos/screenHeight;
    Serial.print("Ab 1 x web: ");
    Serial.println(ab1WebX);
    Serial.print("Ab 1 y web: ");
    Serial.println(ab1WebY);
    Serial.print("Ab 2 x web: ");
    Serial.println(ab2WebX);
    Serial.print("Ab 2 y web: ");
    Serial.println(ab2WebY);
    Serial.print("Ab 3 x web: ");
    Serial.println(ab3WebX);
    Serial.print("Ab 2 y web: ");
    Serial.println(ab3WebY);

    checkResistance();
  if(webControlCenter == "history"){
    Serial.println("We are in history mode!");
    historyCondition();
  }else if(webControlCenter == "reading"){
    Serial.println("We are in reading mode!");
    readingMode();
  } else{Serial.println("Error in state11");}
}

void state6(){
  //Save the experience
    historyBase(500);
}



/*
    -----------------------------------------------------------
                          DRAW BACTERIA
    -----------------------------------------------------------
*/

void drawingLoop(){
  for(int i=0; i<=lenBactSpreadY; i++){
    int a;
    double b;
    //This If statement is required to convert the for loop value into the two other values that I need
    if (i==0){a=-10;b=-1.0;}
      else if (i==1){a=-9;b=-0.9;}
      else if (i==2){a=-8;b=-0.8;}
      else if (i==3){a=-7;b=-0.7;}
      else if (i==4){a=-6;b=-0.6;}
      else if (i==5){a=-5;b=-0.5;}
      else if (i==6){a=-4;b=-0.4;}
      else if (i==7){a=-3;b=-0.3;}
      else if (i==8){a=-2;b=-0.2;}
      else if (i==9){a=-1;b=-0.1;}
      else if (i==10){a=0;b=0.0;}
      else if (i==11){a=1;b=0.1;}
      else if (i==12){a=2;b=0.2;}
      else if (i==13){a=3;b=0.3;}
      else if (i==14){a=4;b=0.4;}
      else if (i==15){a=5;b=0.5;}
      else if (i==16){a=6;b=0.6;}
      else if (i==17){a=7;b=0.7;}
      else if (i==18){a=8;b=0.8;}
      else if (i==19){a=9;b=0.9;}
      else if (i==20){a=10;b=1.0;}
      else if (i==21){a=11;b=1.1;}
    drawBacteria_y(i, a, b); //Draw rect function
  }

  for(int h=0; h<=lenBactSpreadX; h++){
    int a;
    double b;
    //This If statement is required to convert the for loop value into the two other values that I need
    if (h==0){a=-10;b=-1.0;}
      else if (h==1){a=-9;b=-0.9;}
      else if (h==2){a=-8;b=-0.8;}
      else if (h==3){a=-7;b=-0.7;}
      else if (h==4){a=-6;b=-0.6;}
      else if (h==5){a=-5;b=-0.5;}
      else if (h==6){a=-4;b=-0.4;}
      else if (h==7){a=-3;b=-0.3;}
      else if (h==8){a=-2;b=-0.2;}
      else if (h==9){a=-1;b=-0.1;}
      else if (h==10){a=0;b=0.0;}
      else if (h==11){a=1;b=0.1;}
      else if (h==12){a=2;b=0.2;}
      else if (h==13){a=3;b=0.3;}
      else if (h==14){a=4;b=0.4;}
      else if (h==15){a=5;b=0.5;}
      else if (h==16){a=6;b=0.6;}
      else if (h==17){a=7;b=0.7;}
      else if (h==18){a=8;b=0.8;}
      else if (h==19){a=9;b=0.9;}
      else if (h==20){a=10;b=1.0;}
      else if (h==21){a=11;b=1.1;}
    drawBacteria_x(h, a, b); //Draw rect function
  }
}

void drawBacteria_y(int j, int k, double w){
  //Statement that check the accellerometer Position and if the line was already drawn or not
  if(::bacteriaSpreadY[j] == 0 && accelValueY >= (0.0+w) && accelValueY <= (0.09+w)){
      //Function to draw the rectangle
        display.fillRect(::xLeftPetri, ::yCenter-(k*::bacteriaLinesStep)-(bacteriaZone/2 + 10), bacteriaZone, bacteriaZone*2 + 10, BLACK);
        for (int i=2; i<=40; i+=2){display.fillRect(xLeftPetri + (bacteriaZone * i), ::yCenter-(k*::bacteriaLinesStep)-(bacteriaZone/2 + 10), bacteriaZone, bacteriaZone*2 + 10, BLACK);}
      //Partial update to the screen (check that this works only with BLACK / WHITE color and not with 0 / 1
        display.partialUpdate();
      ::bacteriaSpreadY[j] = 1;
  } 
}

void drawBacteria_x(int j, int k, double w){
  //Statement that check the accellerometer Position and if the line was already drawn or not
  if(::bacteriaSpreadX[j] == 0 && accelValueX >= (0.0+w) && accelValueX <= (0.09+w)){
      //Function to draw the rectangle
        display.fillRect(::xCenter+(k*::bacteriaLinesStep)+(bacteriaZone/2), 0, bacteriaZone, screenHeight, BLACK);
      //Partial update to the screen (check that this works only with BLACK / WHITE color and not with 0 / 1
        display.partialUpdate();
      ::bacteriaSpreadX[j] = 1;
  }  
}

void checkLoopSpreading(){
  if(bacteriaSpreadY[0]+bacteriaSpreadY[1]+bacteriaSpreadY[2]+bacteriaSpreadY[3]+bacteriaSpreadY[4]+bacteriaSpreadY[5]+bacteriaSpreadY[6]+bacteriaSpreadY[7]+bacteriaSpreadY[8]+bacteriaSpreadY[9]+bacteriaSpreadY[10]+bacteriaSpreadY[11]+bacteriaSpreadY[12]+bacteriaSpreadY[13]+bacteriaSpreadY[14]+bacteriaSpreadY[15]+bacteriaSpreadY[16]+bacteriaSpreadY[17]+bacteriaSpreadY[18]+bacteriaSpreadY[19]+bacteriaSpreadY[20] == 21){
     completeBactSpreadY = true;
  } else {
     completeBactSpreadY = false;
  }

  if(bacteriaSpreadX[0]+bacteriaSpreadX[1]+bacteriaSpreadX[2]+bacteriaSpreadX[3]+bacteriaSpreadX[4]+bacteriaSpreadX[5]+bacteriaSpreadX[6]+bacteriaSpreadX[7]+bacteriaSpreadX[8]+bacteriaSpreadX[9]+bacteriaSpreadX[10]+bacteriaSpreadX[11]+bacteriaSpreadX[12]+bacteriaSpreadX[13]+bacteriaSpreadX[14]+bacteriaSpreadX[15]+bacteriaSpreadX[16]+bacteriaSpreadX[17]+bacteriaSpreadX[18]+bacteriaSpreadX[19]+bacteriaSpreadX[20] == 21){
     completeBactSpreadX = true;
  } else {
     completeBactSpreadX = false;
  }

  if (completeBactSpreadX == true && completeBactSpreadY == true){
    Serial.println("Everything is covered!");
    arrayCompletedSpreading = true;
  }
}




/*
    -----------------------------------------------------------
              DEVICE READY TO COLLECT TOUCHSCREEN INFO
    -----------------------------------------------------------
*/

void abPlacing(){
  // Check if there is any touch detected
    if (display.tsAvailable()){
        display.setTextColor(BLACK, WHITE);
        
        if (antibioticsCreated < 3){
            uint8_t n;
            uint16_t x[2], y[2];
            // See how many fingers are detected (max 2) and copy x and y position of each finger on touchscreen
            n = display.tsGetData(x, y);
            if (n != 0){
                Serial.println("Touch on screen detected!");
                if (antibioticsCreated == 1){
                    oldCoordX1 = ab1XPos - x[0];
                    oldCoordY1 = ab1YPos - y[0];
                } else if (antibioticsCreated == 2){
                    oldCoordX2 = ab2XPos - x[0];
                    oldCoordY2 = ab2YPos - y[0];
                    oldCoordX1 = ab1XPos - x[0];
                    oldCoordY1 = ab1YPos - y[0];
                }            
                
                if ((!(oldCoordX1 >= -tresholdAntibioticPosition && oldCoordX1 <= tresholdAntibioticPosition) ||
                    !(oldCoordY1 >= -tresholdAntibioticPosition && oldCoordY1 <= tresholdAntibioticPosition)) ||
                    (oldCoordX1 == 0 && oldCoordY1 == 0)) {
                        
                        if ((!(oldCoordX2 >= -tresholdAntibioticPosition && oldCoordX2 <= tresholdAntibioticPosition) ||
                            !(oldCoordY2 >= -tresholdAntibioticPosition && oldCoordY2 <= tresholdAntibioticPosition)) ||
                            (oldCoordX2 == 0 && oldCoordY2 == 0)) {
                                display.fillCircle(x[0], y[0], abDiameter, WHITE);
                                display.setCursor(x[0] - abDiameter/2, y[0] + abDiameter/3);
                                if (antibioticsCreated == 0) {display.println("1");} else if (antibioticsCreated == 1){display.println("2");} else if (antibioticsCreated == 2){display.println("3");}
                                display.partialUpdate();
                                if (antibioticsCreated == 0){
                                ab1XPos = x[0];
                                ab1YPos = y[0];
                                } else if (antibioticsCreated == 1){
                                ab2XPos = x[0];
                                ab2YPos = y[0];
                                } else if (antibioticsCreated == 2){
                                ab3XPos = x[0];
                                ab3YPos = y[0];
                                }
                                antibioticsCreated += 1;
                                Serial.println(antibioticsCreated);
                        } else {
                        Serial.println("Error generating AB - Code 2");
                        errorCheck(x[0], y[0]);}
                } else {
                        Serial.println("Error generating AB - Code 1");
                        errorCheck(x[0], y[0]);}
            }
        } else if (antibioticsCreated == 3){
            String jsonString = "";
            StaticJsonDocument<200> doc;                      // create a JSON container
            JsonObject object = doc.to<JsonObject>();         // create a JSON Object

            object["ab_placed"] = 1;                          // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else

            serializeJson(doc, jsonString);                   // convert JSON object to string
            Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
            webSocket.broadcastTXT(jsonString);               // send JSON string to clients
            
            Serial.println("Antibiotic placed correctly");
            Serial.println(antibioticsCreated);
        }
    }
}

void errorCheck(float xCoord, float yCoord){
  Serial.print("Previous coordinate X: ");
  Serial.println(ab1XPos);
  Serial.print("Previous coordinate Y: ");
  Serial.println(ab1YPos);
  Serial.print("Coordinate X: ");
  Serial.println(xCoord);
  Serial.print("Coordinate Y: ");
  Serial.println(yCoord);
  Serial.print("Calc X 1: ");
  Serial.println(oldCoordX1);
  Serial.print("Calc Y 1: ");
  Serial.println(oldCoordY1);
  Serial.print("Calc X 2: ");
  Serial.println(oldCoordX2);
  Serial.print("Calc Y 2: ");
  Serial.println(oldCoordY2);
}



/*
    -----------------------------------------------------------
                          CONTROL CENTER
    -----------------------------------------------------------
*/

void readingMode(){
    display.clearDisplay();
    display.setTextSize(3);
    display.fillScreen(BLACK);
    display.setTextColor(BLACK, WHITE);

    Serial.print("AB1 Resistance: ");
    Serial.print(ab1Resistance);
    Serial.print("   AB2 Resistance: ");
    Serial.print(ab2Resistance);
    Serial.print("   AB3 Resistance: ");
    Serial.println(ab3Resistance);

    if (webReadingAB == 1){

        for(int i=0; i<500; i++){
            display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 4, WHITE);
            }

        display.fillCircle(xCenter, yCenter, ab1Resistance*abDiameter, BLACK);
        display.fillCircle(xCenter, yCenter, abDiameter, WHITE);
        display.setCursor(xCenter - abDiameter/2, yCenter + abDiameter/3);
        display.println("1");
        display.display();

    } else if (webReadingAB == 2){

        for(int i=0; i<500; i++){
          display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 4, WHITE);
        }

        display.fillCircle(xCenter, yCenter, ab2Resistance*abDiameter, BLACK);
        display.fillCircle(xCenter, yCenter, abDiameter, WHITE);
        display.setCursor(xCenter - abDiameter/2, yCenter + abDiameter/3);
        display.println("2");
        display.display();

    } else if (webReadingAB == 3){
        
        for(int i=0; i<500; i++){
          display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 4, WHITE);
        }

        display.fillCircle(xCenter, yCenter, ab3Resistance*abDiameter, BLACK);
        display.fillCircle(xCenter, yCenter, abDiameter, WHITE);
        display.setCursor(xCenter - abDiameter/2, yCenter + abDiameter/3);
        display.println("3");
        display.display();

    } else {
        Serial.println("Reading AB not selected!");
    }
}

//nDots is the numbers of dots representing bacteria to generate values can be from 0 to 500 max
void historyBase(int nDots){
    display.clearDisplay();
    display.setTextSize(3);
    display.fillScreen(BLACK);
    display.setTextColor(BLACK, WHITE);
    
    for(int i=0; i<nDots; i++){
      display.fillCircle(bacteriaDotsX[i], bacteriaDotsY[i], 4, WHITE);
    }

      display.fillCircle(ab1XPos, ab1YPos, ab1Resistance*abDiameter, BLACK);
      display.fillCircle(ab2XPos, ab2YPos, ab2Resistance*abDiameter, BLACK);
      display.fillCircle(ab3XPos, ab3YPos, ab3Resistance*abDiameter, BLACK);
    display.fillCircle(ab1XPos, ab1YPos, abDiameter, WHITE);
        display.setCursor(ab1XPos - abDiameter/2, ab1YPos + abDiameter/3);
        display.println("1");
    display.fillCircle(ab2XPos, ab2YPos, abDiameter, WHITE);
        display.setCursor(ab2XPos - abDiameter/2, ab2YPos + abDiameter/3);
        display.println("2");
    display.fillCircle(ab3XPos, ab3YPos, abDiameter, WHITE);
        display.setCursor(ab3XPos - abDiameter/2, ab3YPos + abDiameter/3);
        display.println("3");
    display.display();
}

void historyCondition(){
  if(webReadingAB == 0){ //0h
    Serial.println("History mode: 0h !");
    historyBase(0);
  } else if(webReadingAB == 8){ //8h
    Serial.println("History mode: 8h !");
    historyBase(100);
  } else if(webReadingAB == 12){ //12h
    Serial.println("History mode: 12h !");
    historyBase(250);
  } else if(webReadingAB == 24){ //24h
    Serial.println("History mode: 24h !");
    historyBase(500);
  }
}




/*
    -----------------------------------------------------------
                  CALCULATE RESISTANCE VALUE
    -----------------------------------------------------------
*/

int ciprofloxacin[3] = {13, 21, 13};                        //Resistance values for ciprofloxacin
int doxycycline[3] = {15, 10, 13};                          //Resistance values for doxycycline
int gatifloxacin[3] = {17, 21, 21};                          //Resistance values for gatifloxacin
int levofloxacin[3] = {20, 20, 20};                         //Resistance values for levofloxacin
int minocycine[3] = {10, 18, 10};                           //Resistance values for minocycine
int tetracycline[3] = {9, 12, 14};                         //Resistance values for tetracycline

void checkResistance(){
  Serial.println("Data reading:");
  Serial.print("  - Bacteria: ");
  Serial.println(webBacteria);
  Serial.print("  - Antibiotic 1: ");
  Serial.println(webAb1);
  Serial.print("  - Antibiotic 2: ");
  Serial.println(webAb2);
  Serial.print("  - Antibiotic 3: ");
  Serial.println(webAb3);
/*
  if(j_bacteria == "Acinetobacter"){
    if (j_ab1 == "Ciprofloxacin"){ ab1_resistance = ab_resistance_multiplier * ciprofloxacin[0];};
    if (j_ab1 == "Doxycycline"){ ab1_resistance = ab_resistance_multiplier * doxycycline[0];};
    if (j_ab1 == "Gatifloxacin"){ ab1_resistance = ab_resistance_multiplier * gatifloxacin[0];};
    if (j_ab1 == "Levofloxacin"){ ab1_resistance = ab_resistance_multiplier * levofloxacin[0];};
    if (j_ab1 == "Minocycine"){ ab1_resistance = ab_resistance_multiplier * minocycine[0];};
    if (j_ab1 == "Tetracycline"){ ab1_resistance = ab_resistance_multiplier * tetracycline[0];};

    if (j_ab2 == "Ciprofloxacin"){ ab2_resistance = ab_resistance_multiplier * ciprofloxacin[0];};
    if (j_ab2 == "Doxycycline"){ ab2_resistance = ab_resistance_multiplier * doxycycline[0];};
    if (j_ab2 == "Gatifloxacin"){ ab2_resistance = ab_resistance_multiplier * gatifloxacin[0];};
    if (j_ab2 == "Levofloxacin"){ ab2_resistance = ab_resistance_multiplier * levofloxacin[0];};
    if (j_ab2 == "Minocycine"){ ab2_resistance = ab_resistance_multiplier * minocycine[0];};
    if (j_ab2 == "Tetracycline"){ ab2_resistance = ab_resistance_multiplier * tetracycline[0];};
    
    if (j_ab3 == "Ciprofloxacin"){ ab3_resistance = ab_resistance_multiplier * ciprofloxacin[0];};
    if (j_ab3 == "Doxycycline"){ ab3_resistance = ab_resistance_multiplier * doxycycline[0];};
    if (j_ab3 == "Gatifloxacin"){ ab3_resistance = ab_resistance_multiplier * gatifloxacin[0];};
    if (j_ab3 == "Levofloxacin"){ ab3_resistance = ab_resistance_multiplier * levofloxacin[0];};
    if (j_ab3 == "Minocycine"){ ab3_resistance = ab_resistance_multiplier * minocycine[0];};
    if (j_ab3 == "Tetracycline"){ ab3_resistance = ab_resistance_multiplier * tetracycline[0];};
  } else if(j_bacteria == "Enterococcus"){
    if (j_ab1 == "Ciprofloxacin"){ ab1_resistance = ab_resistance_multiplier * ciprofloxacin[1];};
    if (j_ab1 == "Doxycycline"){ ab1_resistance = ab_resistance_multiplier * doxycycline[1];};
    if (j_ab1 == "Gatifloxacin"){ ab1_resistance = ab_resistance_multiplier * gatifloxacin[1];};
    if (j_ab1 == "Levofloxacin"){ ab1_resistance = ab_resistance_multiplier * levofloxacin[1];};
    if (j_ab1 == "Minocycine"){ ab1_resistance = ab_resistance_multiplier * minocycine[1];};
    if (j_ab1 == "Tetracycline"){ ab1_resistance = ab_resistance_multiplier * tetracycline[1];};

    if (j_ab2 == "Ciprofloxacin"){ ab2_resistance = ab_resistance_multiplier * ciprofloxacin[1];};
    if (j_ab2 == "Doxycycline"){ ab2_resistance = ab_resistance_multiplier * doxycycline[1];};
    if (j_ab2 == "Gatifloxacin"){ ab2_resistance = ab_resistance_multiplier * gatifloxacin[1];};
    if (j_ab2 == "Levofloxacin"){ ab2_resistance = ab_resistance_multiplier * levofloxacin[1];};
    if (j_ab2 == "Minocycine"){ ab2_resistance = ab_resistance_multiplier * minocycine[1];};
    if (j_ab2 == "Tetracycline"){ ab2_resistance = ab_resistance_multiplier * tetracycline[1];};
    
    if (j_ab3 == "Ciprofloxacin"){ ab3_resistance = ab_resistance_multiplier * ciprofloxacin[1];};
    if (j_ab3 == "Doxycycline"){ ab3_resistance = ab_resistance_multiplier * doxycycline[1];};
    if (j_ab3 == "Gatifloxacin"){ ab3_resistance = ab_resistance_multiplier * gatifloxacin[1];};
    if (j_ab3 == "Levofloxacin"){ ab3_resistance = ab_resistance_multiplier * levofloxacin[1];};
    if (j_ab3 == "Minocycine"){ ab3_resistance = ab_resistance_multiplier * minocycine[1];};
    if (j_ab3 == "Tetracycline"){ ab3_resistance = ab_resistance_multiplier * tetracycline[1];};
  } else if(j_bacteria == "Entrobacteriaceae"){
    if (j_ab1 == "Ciprofloxacin"){ ab1_resistance = ab_resistance_multiplier * ciprofloxacin[2];};
    if (j_ab1 == "Doxycycline"){ ab1_resistance = ab_resistance_multiplier * doxycycline[2];};
    if (j_ab1 == "Gatifloxacin"){ ab1_resistance = ab_resistance_multiplier * gatifloxacin[2];};
    if (j_ab1 == "Levofloxacin"){ ab1_resistance = ab_resistance_multiplier * levofloxacin[2];};
    if (j_ab1 == "Minocycine"){ ab1_resistance = ab_resistance_multiplier * minocycine[2];};
    if (j_ab1 == "Tetracycline"){ ab1_resistance = ab_resistance_multiplier * tetracycline[2];};

    if (j_ab2 == "Ciprofloxacin"){ ab2_resistance = ab_resistance_multiplier * ciprofloxacin[2];};
    if (j_ab2 == "Doxycycline"){ ab2_resistance = ab_resistance_multiplier * doxycycline[2];};
    if (j_ab2 == "Gatifloxacin"){ ab2_resistance = ab_resistance_multiplier * gatifloxacin[2];};
    if (j_ab2 == "Levofloxacin"){ ab2_resistance = ab_resistance_multiplier * levofloxacin[2];};
    if (j_ab2 == "Minocycine"){ ab2_resistance = ab_resistance_multiplier * minocycine[2];};
    if (j_ab2 == "Tetracycline"){ ab2_resistance = ab_resistance_multiplier * tetracycline[2];};
    
    if (j_ab3 == "Ciprofloxacin"){ ab3_resistance = ab_resistance_multiplier * ciprofloxacin[2];};
    if (j_ab3 == "Doxycycline"){ ab3_resistance = ab_resistance_multiplier * doxycycline[2];};
    if (j_ab3 == "Gatifloxacin"){ ab3_resistance = ab_resistance_multiplier * gatifloxacin[2];};
    if (j_ab3 == "Levofloxacin"){ ab3_resistance = ab_resistance_multiplier * levofloxacin[2];};
    if (j_ab3 == "Minocycine"){ ab3_resistance = ab_resistance_multiplier * minocycine[2];};
    if (j_ab3 == "Tetracycline"){ ab3_resistance = ab_resistance_multiplier * tetracycline[2];};
  }

  ab1_resistanceWeb = 1.6 * 40 * ((ab1_resistance * ab_resistance_multiplier * abDiameter) / 35);
  ab2_resistanceWeb = 1.6 * 40 * ((ab2_resistance * ab_resistance_multiplier * abDiameter) / 35);
  ab3_resistanceWeb = 1.6 * 40 * ((ab3_resistance * ab_resistance_multiplier * abDiameter) / 35);

  Serial.println("");
  Serial.println(" ---------------- ");
  Serial.println("");
  Serial.println("Data produced:");
  Serial.print("  - Resistance value 1: ");
  Serial.println(ab1_resistance);
  Serial.print("  - Resistance web value 1: ");
  Serial.println(ab1_resistanceWeb);
  Serial.print("  - Resistance value 2: ");
  Serial.println(ab2_resistance);
  Serial.print("  - Resistance web value 2: ");
  Serial.println(ab2_resistanceWeb);
  Serial.print("  - Resistance value 3: ");
  Serial.println(ab3_resistance);
  Serial.print("  - Resistance web value 3: ");
  Serial.println(ab3_resistanceWeb);
*/
}