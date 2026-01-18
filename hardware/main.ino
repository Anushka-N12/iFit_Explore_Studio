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

char dbString[] = "....................";
char dbString2[] = "....................";

void setup(){
    Serial.begin(9600); // initialize serial communication
    Wire.begin();
    pinMode(buttonPin, INPUT_PULLUP); 
    pinMode(BUTTON_PIN_AG, INPUT);
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

void nextState()  {
    currentState++; // move to the next state
    if (currentState > 5) { // if the current state exceeds the maximum state, reset to 1
        currentState = 1;
    }
    if (currentState == 3){
        if((wifiMulti.run() == WL_CONNECTED)) {
              HTTPClient http;
              Serial.print("[HTTP] begin...\n");
              String urlString = "http://192.168.1.6:5000/cworkout";
              http.begin(urlString.c_str()); //HTTP
              Serial.print("[HTTP] GET...\n");
              int httpCode = http.GET();
              if(httpCode > 0) {
                  Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                  if(httpCode == HTTP_CODE_OK) {
                      String payload = http.getString();
                      Serial.println(payload.substring(2,5));
                      for (int i = 0; i < 20; i++) {dbString[i] = '.';}
                      int i = 2;
                      while (i < payload.length() && payload[i] != '\"') {
                          dbString[i] = payload[i];
                          i++;
                      }//while
                      i = i + 3; //Skipping "," chars
                      for (int k = 0; k < 20; k++) {dbString2[k] = '.';}
                      int count_next = 0;
                      for (int j = i; j < payload.length()-2; j++){ //Skip "] chars
                          dbString2[count_next] = payload[j];
                          count_next++;
                      }//for
                  } else{}
              } else {
              Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
              http.errorToString(httpCode).c_str();
              }//else
              http.end();
        }//connect
    }//==3
}//void nextState

int totalSeconds;
int minutes;
int seconds;

void loop(){
    if (digitalRead(buttonPin) == HIGH) { // check if the button state has changed
    nextState(); // if pressed, move to the next state
    }//button 1 
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
            if((wifiMulti.run() == WL_CONNECTED)) {
                HTTPClient http;
                Serial.print("[HTTP] begin...\n");
                http.begin("http://192.168.0.104:5000/time"); //HTTP
                Serial.print("[HTTP] GET...\n");
                int httpCode = http.GET();
                if(httpCode > 0) {
                    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                    if(httpCode == HTTP_CODE_OK) {
                        String payload = http.getString();
                        tft.fillScreen(TFT_BLACK);
                        tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
                        tft.setTextColor(TFT_WHITE);
                        tft.setCursor(80, 60, 4);
                        tft.println(payload.substring(2,7)+"\n");
                        Serial.println(payload.substring(2,7));
                        tft.setCursor(40, 120, 4);
                        tft.println(payload.substring(14,28));
                        Serial.println(payload.substring(14,28));
                    } else {
                        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                    }//else OK
                }//0
                http.end();
            }//connect
            break;
        case 3:
            tft.fillScreen(TFT_BLACK);
            tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
            if (digitalRead(BUTTON_PIN_AG) == LOW && !buttonPressed) {
                buttonPressed = true;
                timerRunning = !timerRunning;
                if (timerRunning) {
                     startTime = millis();
                } else {
                  if((wifiMulti.run() == WL_CONNECTED)) {
                    HTTPClient http;
                    Serial.print("Sending");
                    Serial.print("...\n");
                    Serial.print("[HTTP] begin...\n");
                    http.begin("http://192.168.0.104:5000/pred");            
                    Serial.print("[HTTP] GET...\n");
                    int httpCode = http.GET();
                    if(httpCode > 0) {
                        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                        if(httpCode == HTTP_CODE_OK) {
                            String payload = http.getString();
                            Serial.println(payload);
                        }//OK
                    } else {
                        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                    }//else0
                    http.end();
                  } else{Serial.println("Failed to begin communication");}//else connect
                }//timerRunning
            } else if (digitalRead(BUTTON_PIN_AG) == HIGH && buttonPressed) {
                buttonPressed = false;
            }//HIGH
            if (timerRunning) {
                elapsedTime = millis() - startTime;
                Serial.println(count_ag);
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
                    String urlString = "http://192.168.0.104:5000/sendArgs?ag_data=" + String(cdata);
                    http.begin(urlString.c_str());            
                    Serial.print("[HTTP] GET...\n");
                    int httpCode = http.GET();
                    if(httpCode > 0) {
                        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                        if(httpCode == HTTP_CODE_OK) {
                            String payload = http.getString();
                            Serial.println(payload);
                        }//OK
                    } else {
                        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                    }//else0
                    http.end();
                } else{Serial.println("Failed to begin communication");}//else connect
                char timeString[16];
                sprintf(timeString, "%02d:%02d", minutes, seconds);
                tft.fillScreen(TFT_BLACK);
                tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
                tft.setTextColor(TFT_WHITE);
                tft.setCursor(50, 50, 4);
                tft.println(dbString);
                tft.setCursor(90, 100, 4);
                tft.println(timeString);
                tft.setCursor(50, 150, 4);
                tft.println(dbString2);
                delay(1000);
            }//timerRunning    
            count_ag += 1;
            if (count_ag % 2 == 0){
                totalSeconds = elapsedTime / 1000;
                minutes = totalSeconds / 60;
                seconds = totalSeconds % 60;
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
                tft.println(dbString);
                tft.setCursor(90, 100, 4);
                tft.println(timeString);
                tft.setCursor(50, 150, 4);
                tft.println(dbString2);
                count_ag = 0;
            }//ag==0
            break;
        case 4:
            tft.fillScreen(TFT_BLACK);
            tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
            if (digitalRead(BUTTON_PIN_AG) == LOW && !buttonPressed) {
                buttonPressed = true;
                timerRunning = !timerRunning;
                if (timerRunning) {
                    startTime = millis();
                } else {}
            } else if (digitalRead(BUTTON_PIN_AG) == HIGH && buttonPressed) {
                buttonPressed = false;
            }//elseHIGH
            if (timerRunning) {
                elapsedTime = millis() - startTime;
                Serial.println(count_ag);
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
                    String urlString = "http://192.168.0.104:5000/sendArgs?ag_data=" + String(cdata);
                    http.begin(urlString.c_str());            
                    Serial.print("[HTTP] GET...\n");
                    int httpCode = http.GET();
                    if(httpCode > 0) {
                        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                        if(httpCode == HTTP_CODE_OK) {
                            String payload = http.getString();
                            Serial.println(payload);
                        }//OK
                    } else {
                        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                    }//else0 
                    http.end();
                } else{Serial.println("Failed to begin communication");}//elseconnect
                char timeString[16];
                sprintf(timeString, "%02d:%02d", minutes, seconds);
                tft.fillScreen(TFT_BLACK);
                tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
                tft.setTextColor(TFT_WHITE);
                tft.setCursor(70, 80, 4);
                tft.println("Add Workout");
                tft.setCursor(90, 120, 4);
                tft.println(timeString);
                delay(1000);
            }//running
            count_ag += 1;
            if (count_ag % 2 == 0){
                totalSeconds = elapsedTime / 1000;
                minutes = totalSeconds / 60;
                seconds = totalSeconds % 60;
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
                tft.setCursor(70, 80, 4);
                tft.println("Add Workout");
                tft.setCursor(90, 120, 4);        
                tft.println(timeString);
                count_ag = 0;
            }//ag
            break;
    }//switch
    delay(1000);
}

