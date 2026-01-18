#include <WiFi.h>
#include <Wire.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <stdio.h>

WiFiMulti wifiMulti;
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
  wifiMulti.addAP("blah", "blah");
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
      break;
    case 2:
      Serial.println("State 3: Perform action 2");
      break;
    case 3:

      // Check button state
      if (digitalRead(BUTTON_PIN_AG) == LOW && !buttonPressed) {
        // Button was pressed and not yet released
        buttonPressed = true;
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
        // if (count_ag == 5){
          Serial.println("Recording data");
          sensors_event_t a, g, temp;
          mpu.getEvent(&a, &g, &temp);

          data[0] = a.acceleration.x;
          Serial.println(a.acceleration.x);
          data[1] = a.acceleration.y;
          data[2] = a.acceleration.z;

          data[3] = a.gyro.x;
          data[4] = a.gyro.y;
          data[5] = a.gyro.z;

          sprintf(cdata, "[%f,%f,%f,%f,%f,%f]", data[0], data[1], data[2], data[3], data[4], data[5]);
          Serial.println(cdata);
          
          Serial.println("Trying to connect to wifi now");
          if((wifiMulti.run() == WL_CONNECTED)) {
            HTTPClient http;
            Serial.print("Sending");
            Serial.print("...\n");
            Serial.print("[HTTP] begin...\n");
            String urlString = "http://192.168.1.6:5000/sendArgs?ag_data=" + String(cdata);
            Serial.println(urlString);
            http.begin(urlString.c_str());            
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
                Serial.println(payload);
                // delay(1000);
              }
            } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
            http.end();
          } else{Serial.println("Failed to begin communication");}
        // }

        char timeString[16];
        sprintf(timeString, "%02d:%02d", minutes, seconds);
      }
      count_ag += 1;
      if (count_ag % 2 == 0){
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
        count_ag = 0;
      }
      break;
  }
  delay(100); // debounce delay to prevent bouncing
}
