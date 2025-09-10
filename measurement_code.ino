   // // Including libraries for SD Card, LOAD cell & RTC:
#include "HX711.h"
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"
#define PIN_CS 2 // The SD card CS is connected to digital pin no. 2 of Arduino board
#define FILE_NAME "Project2.txt" // File name in SD card is kept as 'Lab9.txt'
#define DHTPIN 9 // The data pin of DHT11/DHT22 should be connected to the digital pin 9 of Arduino.
#define DHTTYPE DHT22
const int LOADCELL_DOUT_PIN = A0; // Data pin
const int LOADCELL_SCK_PIN = 4;    // Clock pin
RTC_DS3231 rtc; 
File myFile; 
HX711 scale;
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
 
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Stay here if RTC is not found
  }

   // Uncomment to manually set the time (YYYY, MM, DD, HH, MM, SS):
    // rtc.adjust(DateTime(2024, 11, 06, 21, 38, 10)); // Set to specific date and time

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    // The following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
   
  }

  dht.begin();
  
  // Setting SD card module:
  if (!SD.begin(PIN_CS)) {
    Serial.println("initialization failed!");
    while (1); // don't do anything more:
  }
  Serial.println("initialization done.");
  
}

void loop() {
  
  // for zero weight

  
    float LCV = scale.read_average(20); //load cell value
    float WEIGHT = 0.0043*(scale.read_average(20))-912.04;
    float humidity = dht.readHumidity(); // will take humidity value
    float temperature = dht.readTemperature(); // will take temperature value
    DateTime now = rtc.now();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Sensor is not available right now");
    } else {
      
    
    Serial.print(now.hour());
   Serial.print(':');
   Serial.print(now.minute());
   Serial.print(':');
   Serial.print(now.second());
   Serial.print("   ");
   Serial.print(now.day());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.year());
  Serial.print("   ");
  Serial.print("Temperature   ");
  Serial.print(temperature);
  Serial.print("*C  ");
  Serial.print("humidity  ");
  Serial.print(humidity);
  Serial.print("%");
    Serial.print(" RAW VALUE: ");
    Serial.print(LCV);
    Serial.print("   ");
    Serial.print(" MEASURED WEIGHT: ");
    Serial.print(WEIGHT);
    Serial.println(" g");

  delay(15000);
  
//open file to write data
 myFile = SD.open("Project3", FILE_WRITE);
  if (myFile) {
    myFile.print(now.hour());
   myFile.print(':');
   myFile.print(now.minute());
   myFile.print(':');
   myFile.print(now.second());
   myFile.print("   ");
   myFile.print(now.day());
  myFile.print('/');
  myFile.print(now.month());
  myFile.print('/');
  myFile.print(now.year());
  myFile.print("   ");
  myFile.print("Temperature   ");
  myFile.print(temperature);
  myFile.print(" *C ");
  myFile.print("humidity  ");
  myFile.print(humidity);
  myFile.print(" % ");
    myFile.print(" RAW VALUE: ");
    myFile.print(LCV);
    myFile.print("   ");
    myFile.print(" MEASURED WEIGHT: ");
    myFile.print(WEIGHT);
    myFile.println(" g");

delay(15000);
myFile.close();
  }
  else {
    Serial.print(F("SD Card: error on opening file "));
    Serial.println("Project3.txt");
  }
  
}
}
