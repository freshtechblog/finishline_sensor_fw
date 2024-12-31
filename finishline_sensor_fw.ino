#include <Wire.h>
#include <Arduino.h>

volatile bool startFlag = false; // start time interrupt flag
volatile bool endFlag = false; // end time interrupt flag
volatile unsigned long elapsedTime = 0; // the calculated elapsed time in milliseconds
volatile unsigned long startTime = 0; // the start time in milliseconds
volatile unsigned long zeroTime = 0; // zero time
volatile bool reportTime = false; // should the time be reported

const int sensorInterruptPin = 2;
const int startTimeInterruptPin = 3;

void setup() {
    Serial.begin(115200);  // Uncomment if using serial debugging
    // while (!Serial) {
    //     ; // Wait for serial port to connect. Needed for Leonardo only
    // }
    Serial.println("HW Connected!");

    Wire.begin(0x08);  // Initialize I2C communication as slave

    pinMode(sensorInterruptPin, INPUT_PULLUP);
    pinMode(startTimeInterruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(sensorInterruptPin), handleSensorInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(startTimeInterruptPin), handleStartInterrupt, FALLING);

    Wire.onRequest(requestEvent);  // Set up the request handler for I2C

    startFlag = false;
}

void loop() {
    
    if (startFlag) { // Check if the start flag is set
        startTime = millis(); // get the start time 
        startFlag = false;
        reportTime = false;
        elapsedTime = 0; 
        Serial.println("start flag triggered");
    }

    if(endFlag) { // check if the end flag is set
        if (reportTime == false) {
            elapsedTime = millis() - startTime;
            reportTime = true;
            Serial.print("Elapsed Time: ");
            Serial.println(elapsedTime);
        }
        Serial.println("end flag triggered");
        endFlag=false;
    }
}


/// @brief Handle the interrupt for proximity detected. Sets the end flag
void handleSensorInterrupt() {
    endFlag = true;
}

/// @brief Handle the interrupt for starting the race. Sets the start flag
void handleStartInterrupt() {
    startFlag = true;
}

/// @brief Handle request for data. Return the elapsed time if the start and 
/// @brief end flags have been set. Otherwise, return zero.
void requestEvent() {
    if (reportTime) {
        Wire.write((byte*)&elapsedTime, sizeof(unsigned long));
    } else {
        unsigned long zeroTime = 0;
        Wire.write((byte*)&zeroTime, sizeof(unsigned long));
    }
}
