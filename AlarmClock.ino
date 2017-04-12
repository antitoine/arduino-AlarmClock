/* AlarmClock */ 

/*********** Includes **********/
#include "TimerOne.h"
#include "SevSeg.h"

/*********** Defines **********/
#define BUTTON_1   0  // Pin connected to the first button
#define BUTTON_2   1  // Pin connected to the second button
#define SEG_A      2  // Pin connected to the segment A
#define SEG_B      3  // Pin connected to the segment B
#define SEG_C      4  // Pin connected to the segment C
#define SEG_D      5  // Pin connected to the segment D
#define SEG_E      6  // Pin connected to the segment E
#define SEG_F      7  // Pin connected to the segment F
#define SEG_G      8  // Pin connected to the segment G
#define SEG_P      9  // Pin connected to the point
#define DIGIT_1    10 // Pin connected to the digit 1
#define DIGIT_2    11 // Pin connected to the digit 2
#define DIGIT_3    12 // Pin connected to the digit 3
#define DIGIT_4    13 // Pin connected to the digit 4

/*********** Global variables **********/
SevSeg sevseg;           // Object of the lib SevSeg in order to control segments
long time = 0;           // The global time of the clock in seconds
int pointPosition = 0;   // Position of the point (change every seconds)

/*********** Setup **********/
void setup() {
  // Set up segments
  byte numDigits = 4;
  byte digitPins[] = {DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4};
  byte segmentPins[] = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_P};
  bool resistorsOnSegments = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);
  
  // Init timer
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(incrementTimer);
  
  // Configure buttons
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
}

/*********** Main loop **********/
void loop() {
  if (!digitalRead(BUTTON_1)) {
    time += 3600;
    checkTime();
    while (!digitalRead(BUTTON_1));
  }
  if (!digitalRead(BUTTON_2)) {
    time += 60;
    checkTime();
    while (!digitalRead(BUTTON_2));
  }
  
  int minutes = (time / 60) % 60;
  int hours = (time / 3600) * 100;
  sevseg.setNumber(hours + minutes, pointPosition);
  sevseg.refreshDisplay();
}

/*********** Functions **********/

// Used by the interruption every seconds
void incrementTimer() {
  time++;
  checkTime();
  pointPosition++;
  if (pointPosition == 4) {
    pointPosition = 0;
  }
}

void checkTime() {
  if (time >= 86400) {
    time = 0;
  }
}
