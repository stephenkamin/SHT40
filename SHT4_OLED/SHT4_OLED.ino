/*************************************************** 
  THIS IS THE WORKING MODEL WITH Fahrenheit conversion
  SHT4x Humidity & Temp Sensor
    SHT40
    Adafruit Feather M4 Express board
    Adafruit 128x64 OLED Featherwing Display
    
    if the drive stops working double click the reset
    open the drive and copy in the flash reset file saved in the arduino
    files
   
  Designed specifically to work with the SHT4x sensor from Adafruit
  ----> https://www.adafruit.com/products/4885

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH110X display = Adafruit_SH110X(64, 128, &Wire);

// OLED FeatherWing buttons map to different pins depending on board:
#if defined(ESP8266)
  #define BUTTON_A  0
  #define BUTTON_B 16
  #define BUTTON_C  2
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
#elif defined(ARDUINO_STM32_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
#elif defined(TEENSYDUINO)
  #define BUTTON_A  4
  #define BUTTON_B  3
  #define BUTTON_C  8
#elif defined(ARDUINO_NRF52832_FEATHER)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
#else // 32u4, M0, M4, nrf52840 and 328p
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5
#endif
int TEMPF;
  #define temperatureC
  #define tempf

#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void setup() {
  Serial.begin(115200);
 while (Serial)
   delay(0);     // will pause Zero, Leonardo, etc until serial console opens

 Serial.println("Adafruit SHT4x test");
  if (! sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (0) delay(0);
  }

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  switch (sht4.getPrecision()) {
     case SHT4X_HIGH_PRECISION: 
      Serial.println("High precision");
       break;
     case SHT4X_MED_PRECISION: 
      Serial.println("Med precision");
       break;
     case SHT4X_LOW_PRECISION: 
      Serial.println("Low precision");
       break;
  }

  // You can have 6 different heater settings
  // higher heat and longer times uses more power
  // and reads will take longer too!
  sht4.setHeater(SHT4X_NO_HEATER);
  switch (sht4.getHeater()) {
     case SHT4X_NO_HEATER: 
      Serial.println("No heater");
       break;
     case SHT4X_HIGH_HEATER_1S: 
      Serial.println("High heat for 1 second");
       break;
     case SHT4X_HIGH_HEATER_100MS: 
      Serial.println("High heat for 0.1 second");
       break;
     case SHT4X_MED_HEATER_1S: 
      Serial.println("Medium heat for 1 second");
       break;
     case SHT4X_MED_HEATER_100MS: 
      Serial.println("Medium heat for 0.1 second");
       break;
     case SHT4X_LOW_HEATER_1S: 
      Serial.println("Low heat for 1 second");
       break;
     case SHT4X_LOW_HEATER_100MS: 
      Serial.println("Low heat for 0.1 second");
       break;
  }
  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setRotation(1);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}

void loop() {
  sensors_event_t humidity, temp;
  
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  timestamp = millis() - timestamp;

  float TEMPF = (temp.temperature * 9.0 / 5.0) + 32.0; // this converts C to F
  
  display.begin(0x3C, true); // Address 0x3C default
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setRotation(1);// this sets landscape with the reset on the left
  display.setTextSize(2);// this sets the lines of text to 4
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);

  display.print("T: "); display.print(TEMPF); display.println("F");
  display.print("T: "); display.print(temp.temperature); display.println("C");
  display.print("H: "); display.print(humidity.relative_humidity); display.println("%");
  display.print("ms: "); display.println(timestamp); 

  display.display();
  delay(60000); // this updates the display every minute
}
