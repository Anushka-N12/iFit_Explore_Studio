const int buttonPin = 2;  // the GPIO pin the button is attached to
int currentState = 0; // variable to store the current state

void setup() {
  Serial.begin(9600); // initialize serial communication
  pinMode(buttonPin, INPUT_PULLUP); // initialize the button pin as input with internal pull-up resistor
}

void nextState() {
  currentState++; // move to the next state
  if (currentState > 4) { // if the current state exceeds the maximum state, reset to 1
    currentState = 1;
  }
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) { // check if the button state has changed
    nextState(); // if pressed, move to the next state
    delay(500); // debounce delay to prevent bouncing
  }
  // Perform actions based on the current state
  switch (currentState) {
    case 1:
      Serial.println("State 1: Perform action 1");
      break;
    case 2:
      Serial.println("State 2: Perform action 2");
      break;
    case 3:
      Serial.println("State 3: Perform action 3");
      break;
    case 4:
      Serial.println("State 4: Perform action 4");
      break;
  }
  delay(100); // debounce delay to prevent bouncing
}
