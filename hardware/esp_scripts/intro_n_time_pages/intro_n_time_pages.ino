#include <WiFi.h>
#include <Wire.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include <stdio.h>

WiFiMulti wifiMulti;
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

// Variables for timer and button state
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool timerRunning = false;
bool buttonPressed = true;
float data[6];
char cdata[50];
unsigned int count_ag = 1;
// Define button pin
#define BUTTON_PIN_AG 15
// Define workout data
const char* currentExercise = "Push-ups";
const char* nextExercise = "Squats";
int repsPerWorkout = 15;

const int buttonPin = 2;  // the GPIO pin the button is attached to
int currentState = 1; // variable to store the current state

String prevMinute = "00"; // Initialize prevMinute variable

void setup() {
  Serial.begin(9600); // initialize serial communication
  Wire.begin();
  pinMode(buttonPin, INPUT_PULLUP); // initialize the button pin as input with internal pull-up resistor
  pinMode(BUTTON_PIN_AG, INPUT); // initialize the button pin as input with internal pull-up resistor
  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  wifiMulti.addAP("Airtel_Chakrapani", "Chitty@1945");

  tft.init();
  tft.fillScreen(TFT_BLACK);
  delay(100);
}

void nextState() {
  currentState++; // move to the next state
  if (currentState > 4) { // if the current state exceeds the maximum state, reset to 1
    currentState = 1;
  }
}

int totalSeconds;
int minutes;
int seconds;

void loop() {
  if (digitalRead(buttonPin) == HIGH) { // check if the button state has changed
    nextState(); // if pressed, move to the next state
    // delay(500); // debounce delay to prevent bouncing
  }
  // Perform actions based on the current state
  switch (currentState) {
    case 1:
      Serial.println("State 1: Perform action 1");
      tft.fillScreen(TFT_BLACK);
      tft.drawCircle(120, 120, 118, TFT_BLUE); // Draw a blue circle
      tft.drawLine(120, 0, 120, 12, TFT_GREEN); // Draw green lines
      tft.drawLine(120, 228, 120, 240, TFT_GREEN);
      tft.drawLine(0, 120, 12, 120, TFT_GREEN);
      tft.drawLine(228, 120, 240, 120, TFT_GREEN);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(70, 60, 4);
      tft.println("iFit Studio");
      tft.setCursor(60, 100, 4);
      tft.println("Welcome!");
      delay(1000);
      nextState();
      break;
    case 2:
      Serial.println("State 2: Perform action 2");
      if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
         String urlString = "http://192.168.1.6:5000/time";
        http.begin(urlString.c_str()); //HTTP


        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
              String payload = http.getString();
              String currentMinute = payload.substring(5, 7);
              Serial.println(payload.substring(2, 7));
              Serial.println(payload.substring(14, 28));
              if (currentMinute != prevMinute) {
                tft.fillScreen(TFT_BLACK);
                tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
                tft.setTextColor(TFT_WHITE);
                tft.setCursor(80, 60, 4);
                tft.println(payload.substring(2, 7) + "\n");
                tft.setCursor(40, 120, 4);
                tft.println(payload.substring(14, 28));
                prevMinute = currentMinute;
              }else{}
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
      }
      break;
    case 3:
     Serial.println("State 3: Perform action 3");
        tft.fillScreen(TFT_BLACK);
        tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(50, 50, 4);
        tft.println("20 squat............");
        tft.setCursor(90, 100, 4);
        tft.println("00:00");
        tft.setCursor(50, 150, 4);
        tft.println("20 treadmill........");

  }
  delay(1000); // debounce delay to prevent bouncing
}
