#include <WiFi.h>
#include <Wire.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <stdio.h>
#include <cstring>

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
char dbString[] = "....................";
char dbString2[] = "....................";
unsigned int count_ag = 1;
// Define button pin
#define BUTTON_PIN_AG 15
// Define workout data
// const char* currentExercise = "Push-ups";
// const char* nextExercise = "Squats";
// int repsPerWorkout = 15;

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

void loop() {
  // Perform actions based on the current state
  switch (currentState) {
    case 1:
      Serial.println("State 1: Perform action 1");
      break;
    case 2:
      Serial.println("State 1: Perform action 1");
      break;
    case 3:
    Serial.println("State 3: Perform action 3");
      tft.fillScreen(TFT_BLACK);
      tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
      
        if((wifiMulti.run() == WL_CONNECTED)) {

          HTTPClient http;

          Serial.print("[HTTP] begin...\n");
          // configure traged server and url
          //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
          String urlString = "http://192.168.1.6:5000/cworkout";
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
              Serial.println(payload.substring(2,5));
              for (int i = 0; i < 20; i++) {dbString[i] = '.';}
              int i = 2;
              while (i < payload.length && payload[i] != '\"') {
                  dbString[i] = payload[i];
                  i++;
              }
              // char* commaPtr = strchr(dbString, ',');
              // int commaIndex;
              // if (commaPtr != nullptr) {
              //   commaIndex = commaPtr - dbString;
              // }
              i = i + 3; //Skipping "," chars
              for (int k = 0; k < 20; k++) {dbString2[k] = '.';}
              int count_next = 0;
              for (int j = i; j < payload.length()-2; j++){ //Skip "] chars
                dbString2[count_next] = payload[j];
                count_next++;
              }
            }else{}
            // }
          } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
          http.end();
        }
        tft.fillScreen(TFT_BLACK);
        tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
        tft.setTextColor(TFT_WHITE);

        tft.setCursor(50, 50, 4);
        tft.println(dbString);
        tft.setCursor(50, 100, 4);
        tft.println(dbString2);
      break;
  }
  delay(20000); // debounce delay to prevent bouncing
}
