/* MIT License 2022 Matteo Subet <hi@zumat.ch>
  
   A Simple code to use a button with ESP32
*/

#define BUTTON_PIN 12 // GIOP13 or GPIO12 or GPIO14 pin connected to button

int lastState = HIGH; // the previous state from the input pin
int currentState;     // the current reading from the input pin
int counter = 0;          // the counter indicating how many times the button is pressed

void setup() {
  Serial.begin(9600); // begin the Serial port
  
  // initialize the pushbutton pin as an pull-up input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);

  if(lastState == LOW && currentState == HIGH){
    counter ++;
    Serial.print("The button is Pressed ");
    Serial.print(counter);
    if(counter < 2)
      Serial.println(" time");
    else
      Serial.println(" times");
    Serial.print("Current state: ");
    Serial.println(currentState);
    Serial.println("-------------------------------");
    Serial.println();
    Serial.println();
  };

  // save the last state
  lastState = currentState;
}