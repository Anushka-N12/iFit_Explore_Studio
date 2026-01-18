
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 4

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte nuidPCC[4];

const int buttonPin = 2;  // the GPIO pin the button is attached to
int currentState = 0; // variable to store the current state

void setup() {
  Serial.begin(9600); // initialize serial communication
  pinMode(buttonPin, INPUT_PULLUP); // initialize the button pin as input with internal pull-up resistor
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void nextState() {
  currentState++; // move to the next state
  if (currentState > 4) { // if the current state exceeds the maximum state, reset to 1
    currentState = 1;
  }
}
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
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
      if(rfid.PICC_IsNewCardPresent() | rfid.PICC_ReadCardSerial()){
        for (byte i = 0; i < 4; i++) {
          nuidPCC[i] = rfid.uid.uidByte[i];
        }
        printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }else{
      Serial.println("State 2: Perform action 2");
      }
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
