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
#define BUZZER     A0 // Pin conncted to the buzzer
#define LED        A3 // Pin connected to the LED

/*********** Global variables **********/
SevSeg sevseg;        // Object of the lib SevSeg in order to control segments
long time = 0;        // The global time of the clock in seconds
int point = false;    // Position of the point (change every seconds)
int milliseconds = 0; // Counter of milliseconds

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
  
  // Init timer clock
  Timer1.initialize(2000); // Interruption every two milliseconds
  Timer1.attachInterrupt(updateTimer);
  
  // Configure buttons
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  
  // Configure melody
  initMelody(BUZZER, LED, 0);
}

/*********** Main loop **********/
void loop() {
  if (!digitalRead(BUTTON_1)) {
    addTime(3600);
    delay(200);
  }
  if (!digitalRead(BUTTON_2)) {
    addTime(60);
    delay(200);
  }
  //sing(1);
}

/*********** Functions **********/

// Used by the interruption every two milmiseconds
void updateTimer() {

  milliseconds += 2;

  // If 1000 ms were counted --> Increment seconds timer
  if (milliseconds == 1000) {
    milliseconds = 0;
    addTime(1);
    point = !point;
  }

  // Refresh the display
  const int minutes = (time / 60) % 60;
  const int hours = (time / 3600) * 100;
  sevseg.setNumber(hours + minutes, point ? 2 : -1);
  sevseg.refreshDisplay();
}

void addTime(int add) {
  time += add;
  if (time >= 86400) {
    time = 0;
  }
}
