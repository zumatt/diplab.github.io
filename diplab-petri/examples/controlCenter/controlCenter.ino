/*
   First example of creating a web page to select bacteria and store the value inside the ESP32.
   
   29 December 2022 by Matteo Subet
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

#include "Inkplate.h"   //Include Inkplate library to the sketch
#include "htmlCode.h"   //Include .h file where we stored out html code of our web page
#include <WebServer.h>  //Include ESP32 library for Web server
#include <WiFi.h>       //Include ESP32 WiFi library
#include <WiFiClient.h> //Include ESP32 WiFi library for AP
#include <uri/UriBraces.h>

#define ssid "DiPLab"
#define pass "password"
#define fwdBtn_PIN 13
#define bckBtn_PIN 12

int lastState_fwd = HIGH;     // the previous state from the fwd button pin
int currentState_fwd;         // the current reading from the fwd input pin
int lastState_bck = HIGH;     // the previous state from the bck button pin
int currentState_bck;         // the current reading from the bck input pin
int counter = 0;              // the counter indicating the state of the button bck (--) / fwd (++)

Inkplate display(INKPLATE_1BIT); // Create an object on Inkplate library and also set library into 1 Bit mode (BW)
WebServer server(80);            // Create Web server on port 80 (HTTP port number)

IPAddress serverIP;
String modality;
int dispW = 1024;                                         //Display Width
int dispH = 758;                                          //Display Height
int petriD = (dispH/2)-10;                                //Petri dish diameter
int xC =  (dispW/2)-(petriD);                             //X position of left side of petri dish
int yC =  (dispH/2)-(petriD);                             //Y position of top side of petri dish
int xC2 = (dispW/2)+(petriD);                             //X position of right side of petri dish
int yC2 = (dispH/2)+(petriD);                             //Y position of bottom side of petri dish

void setup()
{
    Serial.begin(115200);

    pinMode(fwdBtn_PIN, INPUT_PULLUP);
    pinMode(bckBtn_PIN, INPUT_PULLUP);
    
    display.begin();                    // Init Inkplate library (you should call this function ONLY ONCE)
    display.clearDisplay();             // Clear frame buffer of display
    display.display();                  // Put clear image on display
    display.setTextSize(3);             // Scale text to be two times bigger then original (5x7 px)
    display.setTextColor(BLACK, WHITE); // Set text color to black and background color to white
    display.setTextWrap(true);          // If text does not fit on screen, send it to new line

    WiFi.begin();            // Init. WiFi library
    WiFi.mode(WIFI_AP);      // Set WiFi to Access point mode
    WiFi.softAP(ssid, pass); // Set SSID (WiFi name) and password for Access point

    serverIP = WiFi.softAPIP(); // Get the server IP address

    server.on("/", handleRoot); // If you open homepage, go to handle root function
    server.on(UriBraces("/mode/{}"),
              handleString); // If you send some text to Inkplate, go to handleString function. Note that {} brackets at
                             // the end of address. That means that web address has some arguments (our text!).
    server.begin();          // Start the web server
    firstScreen();
}

void loop()
{
    currentState_fwd = digitalRead(fwdBtn_PIN);
    currentState_bck = digitalRead(bckBtn_PIN);

    server.handleClient(); // You have to constantly read if there is any new client connected to web server
    if(modality == "history"){  
      if(lastState_fwd == LOW && currentState_fwd == HIGH){
        if (counter < 3){counter ++;}
        historyCondition();
      }   
      if(lastState_bck == LOW && currentState_bck == HIGH){
        if (counter != 0){counter --;}
        historyCondition();
      }
    } else if(modality == "microscope"){  
      if(lastState_fwd == LOW && currentState_fwd == HIGH){
        if (counter < 3){counter ++;}
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

void updateHTML()
{ // This function will send response to client and send HTML code of our web page
    server.send(200, "text/html", s);
}

void handleRoot()
{ // This function will send response to client if client open a root (homepage) of our web page
    updateHTML();
}

void handleString()
{ // This function will send response to client, send HTML code of web page, get the text from argument sent in web page
  // address and refresh screen with new text
    modality = server.arg("modality");
    updateHTML();
    if (modality == "history"){historyMode();}
    if (modality == "microscope"){microscopeMode();}
    //updatePaper();
}

void updatePaper()
{                              // This function updates screen with new data (text)
    display.clearDisplay();    // Clear everything from epaper frame buffer
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    display.setCursor(xC+100, yC+250);
    display.println("Selected modality:"); // Print out what user typed in web page
    display.setCursor(xC+100, yC+300);
    display.print(modality);
    display.partialUpdate(); // Send everything to screen (refresh the screen)
}


void firstScreen(){
  display.clearDisplay();
  display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
  display.setCursor(xC+100, yC+250);
  display.println("Connect to ");
  display.setCursor(xC+100, yC+300);
  display.print(ssid);
  display.println(" WiFi with pass: ");
  display.setCursor(xC+100, yC+350);
  display.println(pass);
  display.setCursor(xC+100, yC+400);
  display.print("Open web browser and write");
  display.setCursor(xC+100, yC+450);
  display.print("http://");
  display.print(serverIP);
  display.println('/');
  display.display();
}

void microscopeMode(){
    counter = 0;
    baseLayer();

      display.fillCircle(xC2-100, yC+150, 50, 0);
    display.fillCircle(xC2-100, yC+150, 25, 1);
      display.fillCircle(xC+150, yC+450, 100, 0);
    display.fillCircle(xC+150, yC+450, 25, 1);
      display.fillCircle(xC+400, yC+600, 200, 0);
    display.fillCircle(xC+400, yC+600, 25, 1);
    display.display();
}

void historyMode(){
    counter = 0;
    baseLayer();

    display.fillCircle(xC2-100, yC+150, 25, 1);
    display.fillCircle(xC+150, yC+450, 25, 1);
    display.fillCircle(xC+400, yC+600, 25, 1);
    display.display();
}

void baseLayer(){
    display.clearDisplay();
      //Draw base layer
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    for(int i=1; i<=20; i++){  
      display.drawLine(xC+(i*petriD/10), yC, xC+(i*petriD/10), yC2, BLACK);}
    for(int a=1; a<=20; a++){  
      display.drawLine(xC, yC+(a*petriD/10), xC2, yC+(a*petriD/10), BLACK);}
    //END of draw base layer
}

void historyBtn(int j,int k,int l){
    baseLayer();

      display.fillCircle(xC2-100, yC+150, 25*j, 0);
    display.fillCircle(xC2-100, yC+150, 25, 1);
      display.fillCircle(xC+150, yC+450, 25*k, 0);
    display.fillCircle(xC+150, yC+450, 25, 1);
      display.fillCircle(xC+400, yC+600, 25*l, 0);
    display.fillCircle(xC+400, yC+600, 25, 1);
    display.partialUpdate();
}

void historyCondition(){
  if(counter == 0){ //0h
    Serial.println("History mode: 0h !");
    historyMode();
  } else if(counter == 1){ //8h
    Serial.println("History mode: 8h !");
    historyBtn(1.3,2,4);
  } else if(counter == 2){ //12h
    Serial.println("History mode: 12h !");
    historyBtn(1.6,3,6);
  } else if(counter == 3){ //24h
    Serial.println("History mode: 24h !");
    historyBtn(2,4,8);
  }
}

void microscopeCondition(){
  if(counter == 0){ //0x
    Serial.println("Microscope mode: 0x !");
    microscopeMode();
  } else if(counter == 1){ //4x
    Serial.println("Microscope mode: 4x !");
    baseLayer();
      display.fillCircle(dispW/2, dispH/2, 100, 0);
    display.fillCircle(dispW/2, dispH/2, 25, 1);
    display.display();
  } else if(counter == 2){ //10x
    Serial.println("Microscope mode: 10x !");
    baseLayer();
      display.fillCircle(dispW/2, dispH/2, 200, 0);
    display.fillCircle(dispW/2, dispH/2, 50, 1);
    display.display();
  } else if(counter == 3){ //40
    Serial.println("Microscope mode: 40x !");
    baseLayer();
    for(int i=0; i<=500; i++){
      display.fillCircle(random(xC, xC2), random(yC, yC2), 2, 1);
    }
      display.fillCircle(dispW/2, dispH/2, 300, 0);
    display.fillCircle(dispW/2, dispH/2, 75, 1);
    display.display();
  }
}