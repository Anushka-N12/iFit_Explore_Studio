#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
// Define button pin
#define BUTTON_PIN 2

// Variables for timer and button state
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool timerRunning = false;
bool buttonPressed = true;

double dataset[10][6];
double data[6];
unsigned int count_ag = 0;
unsigned int count = 0;

void setup(void){
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(115200);
    delay(100);
}//setup closed

void loop(void){
    if (digitalRead(BUTTON_PIN) == HIGH) {
        delay(2000);
        startTime = millis();
        elapsedTime = millis();
        while (digitalRead(BUTTON_PIN) == LOW) {
            // Toggle timer running state when button is pressed
            timerRunning = !timerRunning;
            // Reset the timer start time if the timer is started
            if (timerRunning) {
                elapsedTime = millis() - startTime;
            } else {}
            // Add a short delay
            delay(100);
            count += 1;
            if (count == 10){
                int totalSeconds = elapsedTime / 1000;
                int minutes = totalSeconds / 60;
                int seconds = totalSeconds % 60;
                // Format and print the time in MM:SS format
                Serial.print("Elapsed time: ");
                Serial.print(minutes < 10 ? "0" : "");
                Serial.print(minutes);
                Serial.print(":");
                Serial.print(seconds < 10 ? "0" : "");
                Serial.println(seconds);
                count = 0;
            }
            else{
                // Serial.println("Recording once");
            }//if count==10
        }//while loop pressed
        delay(2000);
        int totalSeconds = elapsedTime / 1000;
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        // Format and print the time in MM:SS format
        Serial.print("Elapsed time: ");
        Serial.print(minutes < 10 ? "0" : "");
        Serial.print(minutes);
        Serial.print(":");
        Serial.print(seconds < 10 ? "0" : "");
        Serial.println(seconds);
        count = 0;
    }//if button is pressed closed
    else{
        // Serial.println("Button not pressed");
                int totalSeconds = elapsedTime / 1000;
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        // Format and print the time in MM:SS format
        Serial.print("Elapsed time: ");
        Serial.print(minutes < 10 ? "0" : "");
        Serial.print(minutes);
        Serial.print(":");
        Serial.print(seconds < 10 ? "0" : "");
        Serial.println(seconds);
        delay(500);
    }//else button off closed
}//loop closed