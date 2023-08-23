/*
    -----------------------------------------------------------
                    GRAPHICS FOR THE DEVICE UI
    -----------------------------------------------------------
*/
void graphicLanding(){
  //Display all the informations for the WiFi connection
    display.clearDisplay();
    display.setCursor(xC+90, yC+200);
    display.setTextSize(2);
    display.println("Welcome to DiPLab");
    display.setCursor(xC+90, yC+260);
    display.setTextSize(1);
    display.println("the only digital laboratory experience that");
    display.setCursor(xC+90, yC+290);
    display.println("disseminates the issue of Antimicrobial");
    display.setCursor(xC+90, yC+320);
    display.println("Resistance using an interactive tool.");
    display.setCursor(xC+90, yC+390);
    display.print("connect to ");
    display.print(::ssid);
    display.println(" WiFi with the password: ");
    display.setCursor(xC+90, yC+420);
    display.println(::password);
    display.setCursor(xC+90, yC+450);
    display.println("open the web browser and go to");
    display.setCursor(xC+90, yC+480);
    display.print("http://");
    display.print(::serverIP);
    display.println('/');
    
    for (int i=0; i<5; i++){
      display.fillCircle(xC+640+(i*30), yC + 420, 10, BLACK);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xC+610+(i*30), yC + 450, 10, BLACK);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xC+550+(i*30), yC + 480, 10, BLACK);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xC+490+(i*30), yC + 510, 10, BLACK);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xC+340+(i*30), yC + 540, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+280+(i*30), yC + 570, 10, BLACK);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xC+220+(i*30), yC + 600, 10, BLACK);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xC+100+(i*30), yC + 630, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+130+(i*30), yC + 660, 10, BLACK);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xC+160+(i*30), yC + 690, 10, BLACK);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xC+190+(i*30), yC + 720, 10, BLACK);   
    }
    display.setTextSize(2);
    display.display();
}

void graphicConnected(){
    display.clearDisplay();
    display.setCursor(xC+90, yC+200);
    display.setTextSize(2);
    display.println("Your DiPLab is");
    display.setCursor(xC+90, yC+230);
    display.println("connected!");
    
    for (int i=0; i<5; i++){
      display.fillCircle(xC+640+(i*30), yC + 420, 10, BLACK);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xC+610+(i*30), yC + 450, 10, BLACK);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xC+550+(i*30), yC + 480, 10, BLACK);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xC+490+(i*30), yC + 510, 10, BLACK);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xC+340+(i*30), yC + 540, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+280+(i*30), yC + 570, 10, BLACK);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xC+220+(i*30), yC + 600, 10, BLACK);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xC+100+(i*30), yC + 630, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+130+(i*30), yC + 660, 10, BLACK);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xC+160+(i*30), yC + 690, 10, BLACK);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xC+190+(i*30), yC + 720, 10, BLACK);   
    }
    display.setTextSize(2);
    display.display();
}

void graphicNameClasscodeReceived(){
    display.setCursor(xC+90, yC+310);
    display.setTextSize(1);
    display.print("thanks for joining us ");
    display.print(::j_name);
    display.println("!");
    display.setCursor(xC+90, yC+360);
    display.print("your classcode is #");
    display.print(::j_classcode);
    display.partialUpdate();
}

void graphicWaitingBact(){
  //Display all the informations for the WiFi connection
    display.clearDisplay();
  //display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    display.setCursor(xC+90, yC+200);
    display.setTextSize(2);
    display.println("Waiting for bacteria");
    display.setTextSize(1);
    display.setCursor(xC+90, yC+360);
    display.println("select the bacteria with your phone");
    
    for (int i=0; i<5; i++){
      display.fillCircle(xC+640+(i*30), yC + 420, 10, BLACK);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xC+610+(i*30), yC + 450, 10, BLACK);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xC+550+(i*30), yC + 480, 10, BLACK);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xC+490+(i*30), yC + 510, 10, BLACK);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xC+340+(i*30), yC + 540, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+280+(i*30), yC + 570, 10, BLACK);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xC+220+(i*30), yC + 600, 10, BLACK);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xC+100+(i*30), yC + 630, 10, BLACK);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+130+(i*30), yC + 660, 10, BLACK);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xC+160+(i*30), yC + 690, 10, BLACK);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xC+190+(i*30), yC + 720, 10, BLACK);   
    }
    display.setTextSize(2);
    display.display();
}

void graphicWaitingAb(){
    display.clearDisplay();
    display.fillScreen(BLACK);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(xC+90, yC+200);
    display.setTextSize(2);
    display.println("Waiting for antibiotic");
    display.setTextSize(1);
    display.setCursor(xC+90, yC+360);
    display.println("select three antibiotics with your phone");
    
    for (int i=0; i<5; i++){
      display.fillCircle(xC+640+(i*30), yC + 420, 10, WHITE);   
    }
    for (int i=0; i<8; i++){
      display.fillCircle(xC+610+(i*30), yC + 450, 10, WHITE);   
    }
    for (int i=0; i<10; i++){
      display.fillCircle(xC+550+(i*30), yC + 480, 10, WHITE);   
    }
    for (int i=0; i<12; i++){
      display.fillCircle(xC+490+(i*30), yC + 510, 10, WHITE);   
    }
    for (int i=0; i<15; i++){
      display.fillCircle(xC+340+(i*30), yC + 540, 10, WHITE);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+280+(i*30), yC + 570, 10, WHITE);   
    }
    for (int i=0; i<21; i++){
      display.fillCircle(xC+220+(i*30), yC + 600, 10, WHITE);   
    }
    for (int i=0; i<19; i++){
      display.fillCircle(xC+100+(i*30), yC + 630, 10, WHITE);   
    }
    for (int i=0; i<18; i++){
      display.fillCircle(xC+130+(i*30), yC + 660, 10, WHITE);   
    }
    for (int i=0; i<17; i++){
      display.fillCircle(xC+160+(i*30), yC + 690, 10, WHITE);   
    }
    for (int i=0; i<11; i++){
      display.fillCircle(xC+190+(i*30), yC + 720, 10, WHITE);   
    }
    display.setTextSize(2);
    display.display();
}