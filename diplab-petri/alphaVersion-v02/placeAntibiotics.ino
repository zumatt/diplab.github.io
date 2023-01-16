void abShake(){
  if (accX >= 3.00 || accX <= -3.00 || accY >= 3.00 || accY <= -3.00){
    display.clearDisplay();
    display.drawCircle(dispW/2, dispH/2, petriD, BLACK);
    baseLayer();
    
    ab1_x = random(xC+100, xC2-100);
    ab1_y = random(yC+100, yC2-100);
    display.fillCircle(ab1_x, ab1_y, 25, 1);  //First Antibiotic

    ab2_x = random(xC+100, xC2-100);
    ab2_y = random(yC+100, yC2-100);
    display.fillCircle(ab2_x, ab2_y, 25, 1);  //Second Antibiotic

    ab3_x = random(xC+100, xC2-100);
    ab3_y = random(yC+100, yC2-100);
    display.fillCircle(ab3_x, ab3_y, 25, 1);  //Third Antibiotic

    display.display();
    delay(1000);
  }
}
