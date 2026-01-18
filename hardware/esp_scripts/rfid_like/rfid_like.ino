#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // initialize serial communication
  Wire.begin();
  tft.init();
  tft.fillScreen(TFT_BLACK);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(50, 50, 4);
  tft.println("20 squat............");
  tft.setCursor(90, 100, 4);
  tft.println("00:00");
  tft.setCursor(50, 150, 4);
  tft.println("20 treadmill........");
  delay(5000);
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(50, 50, 4);
  tft.println("rowing_machine......");
  tft.setCursor(90, 100, 4);
  tft.println("00:00");
  tft.setCursor(50, 150, 4);
  tft.println("20 squat............");
  delay(5000);
}
