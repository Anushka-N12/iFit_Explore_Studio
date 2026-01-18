#include <WiFi.h>
#include <Wire.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <stdio.h>

WiFiMulti wifiMulti;
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
Adafruit_MPU6050 mpu;

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

const int buttonPin = 2;  // the GPIO pin the button is attached to
int currentState = 1; // variable to store the current state

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
  wifiMulti.addAP("TP-Link_C036", "30615526");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
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
int button_count = 1;
void loop() {
  if (digitalRead(buttonPin) == HIGH) { // check if the button state has changed
    nextState(); // if pressed, move to the next state
  }
  // Perform actions based on the current state
  switch (currentState) {
    case 1:
      Serial.println("State 1: Perform action 1");
      break;
    case 2:
      Serial.println("State 2: Perform action 2");
      tft.fillScreen(TFT_BLACK);
      tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(50, 50, 4);
      tft.println("20 treadmill........");
      tft.setCursor(90, 100, 4);
      tft.println("00:00");
      tft.setCursor(50, 150, 4);
      tft.println("Finished............");
      delay(1000);
      break;
    case 3:
      Serial.println(button_count);
      if (button_count == 3){
        tft.fillScreen(TFT_BLACK);
        tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(50, 50, 4);
        tft.println("Add Workout");
        tft.setCursor(90, 120, 4);
        tft.println("00:00");
        delay(1000);
      }

      tft.fillScreen(TFT_BLACK);
      tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);

      // Check button state
      if (digitalRead(BUTTON_PIN_AG) == LOW && !buttonPressed) {
        // Button was pressed and not yet released
        buttonPressed = true;
        button_count = button_count + 1;

        // Toggle timer running state when button is pressed
        timerRunning = !timerRunning;
        // Reset the timer start time if the timer is started
        if (timerRunning) {
            startTime = millis();
        } else {}
      } else if (digitalRead(BUTTON_PIN_AG) == HIGH && buttonPressed) {
        // Button was released
        buttonPressed = false;
      }
      // Update elapsed time if the timer is running
      if (timerRunning) {
        elapsedTime = millis() - startTime;
        Serial.println(count_ag);
      }
      count_ag += 1;
      if (count_ag % 2 == 0 and button_count != 3){
        totalSeconds = elapsedTime / 1000;
        minutes = totalSeconds / 60;
        seconds = totalSeconds % 60;
        // Format and print the time in MM:SS format
        Serial.print("Elapsed time: ");
        Serial.print(minutes < 10 ? "0" : "");
        Serial.print(minutes);
        Serial.print(":");
        Serial.print(seconds < 10 ? "0" : "");
        Serial.println(seconds);

        char timeString[16];
        sprintf(timeString, "%02d:%02d", minutes, seconds);
        tft.fillScreen(TFT_BLACK);
        tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(50, 50, 4);
        tft.println("Add Workout");
        tft.setCursor(90, 120, 4);
        tft.println(timeString);
        delay(1000);

        count_ag = 0;
      }
      break;
  }
  delay(1000); // debounce delay to prevent bouncing
}
