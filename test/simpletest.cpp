/* 
   edited by genewitch to actually provide clicks per minute on the serial port
   in a sane fashion. the original file i forked from just set CPM to 105 and 
   didn't really work anyhow, as the count was reset every 20 seconds.
   There's an issue with my understanding of the way variables work, so please
   feel free to fix my cludge with the clock1 and start variables. when they 
   were in the loop the "minutes" never changed.

   trying to create PR
*/

/*
   Geiger.ino

   This code interacts with the Alibaba RadiationD-v1.1 (CAJOE) Geiger counter board
   and reports readings in CPM (Counts Per Minute).

   Author: Andreas Spiess

   Based on initial work of Mark A. Heckler (@MkHeck, mark.heckler@gmail.com)

   License: MIT License

   Please use freely with attribution. Thank you!
*/

#include <Arduino.h>

volatile unsigned long counts = 0;                       // Tube events
unsigned long cpm = 0;                                   // CPM
unsigned long previousMillis;                            // Time measurement
const int inputPin = 7;                                  // geiger is on this pin?
unsigned int thirds = 0;
unsigned long minutes = 1;
unsigned long start = 0;

#define LOG_PERIOD 20000 //Logging period in milliseconds
#define MINUTE_PERIOD 60000

void ISR_impulse() { // Captures count of events from Geiger counter board
  counts++;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Booting...");
  Serial.println("Measuring");
  pinMode(inputPin, INPUT);                                                // Set pin for capturing Tube events
  interrupts();                                                            // Enable interrupts
  attachInterrupt(digitalPinToInterrupt(inputPin), ISR_impulse, FALLING); // Define interrupt on falling edge
  unsigned long clock1 = millis();
  start = clock1;
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  
  
  
  if (currentMillis - previousMillis > LOG_PERIOD) {
    
    previousMillis = currentMillis;
    // Serial.print("previousMillis: ");
    // Serial.println(String(previousMillis));
    minutes = (previousMillis - start) / MINUTE_PERIOD;
    if (minutes < 1) {
      minutes = 1;
  }
    // Serial.print("minutes: ");
    // Serial.println(String(minutes));

    //cpm = counts * MINUTE_PERIOD / LOG_PERIOD; this is just counts times 3 so:

    cpm = counts / minutes;
    Serial.print("Total clicks since start: ");    
    Serial.println(String(counts));
    Serial.print("Rolling CPM: ");
    Serial.println(String(cpm));
    
//    if ( thirds > 2) {
//      counts = 0;
//      thirds = 0;  
//    }
    
    
  }
}
  
  
  
