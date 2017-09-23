/* AlarmClock */

/*********** Includes **********/
#include <Wire.h>
#include "TimerOne.h"
#include "SevSeg.h"
#include "RTClib.h"

/*********** Defines **********/
#define BUTTON_1   0  // Pin connected to the first button (need interruption support)
#define BUTTON_2   1  // Pin connected to the second button (need interruption support)
#define SEG_A      2  // Pin connected to the segment A
#define SEG_B      A0  // Pin connected to the segment B
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
SevSeg sevseg;          // Object of the lib SevSeg in order to control segments
int updateTime = false; // Flag: true if time needs to be updated
RTC_DS1307 rtc;         // Real Time Clock instance
DateTime time;          // Current time
DateTime alarmTime;     // Alarm time set up

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
  Timer1.initialize(1000); // Interruption every ms
  Timer1.attachInterrupt(updateTimer);

  // Configure buttons
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);

  // RTC
  Wire.begin();
  rtc.begin();
  if (!rtc.isrunning()) {
    // If RTC not initialised with a time, set one from computer at build time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  time = rtc.now();
}

/*********** Main loop **********/
void loop() {
  static int lastPush = millis();
  static int alarm = false;

  if (!digitalRead(BUTTON_1) && (millis() - lastPush) > 200) {
    alarmTime = alarmTime + 3600;
    lastPush = millis();
    alarm = true;
  }

  if (!digitalRead(BUTTON_2) && (millis() - lastPush) > 200) {
    alarmTime = alarmTime + 60;
    lastPush = millis();
    alarm = true;
  }

  if (updateTime) {
    time = rtc.now();
    updateTime = false;
  }

  int minutes;
  int hours;
  if (alarm && (millis() - lastPush) < 3000) {
    minutes = alarmTime.minute();
    hours = alarmTime.hour();
  } else {
    minutes = time.minute();
    hours = time.hour();
    alarm = false;
  }
  sevseg.setNumber((hours * 100) + minutes, 2);
  sevseg.refreshDisplay();
}

/*********** Functions **********/

// Used by the interruption every ms
void updateTimer() {
  static short milliseconds = 0;
  milliseconds++;
  if (!(milliseconds % 1000)) {
    updateTime = true;
    milliseconds = 0;
  }
}
