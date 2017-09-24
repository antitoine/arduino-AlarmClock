/* AlarmClock */

/*********** Includes **********/
#include <Wire.h>
#include <EEPROM.h>
#include "TimerOne.h"
#include "SevSeg.h"
#include "RTClib.h"

/*********** Defines **********/
#define BUTTON_1   0  // Pin connected to the first button (need interruption support)
#define BUTTON_2   1  // Pin connected to the second button (need interruption support)
#define BUTTON_3   A1 // Pin connected to the third button (activate or deactivate the ring)
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
#define BUZZER     3  // Pin conncted to the buzzer

/*********** Global variables **********/
SevSeg sevseg;                                                     // Object of the lib SevSeg in order to control segments
bool updateTime = false;                                           // Flag: true if time needs to be updated
RTC_DS1307 rtc;                                                    // Real Time Clock instance
DateTime time;                                                     // Current time
DateTime alarmTime(2010, 1, 1, EEPROM.read(0), EEPROM.read(1), 0); // Alarm time set up (hour and minutes saved at EEPROM addresses 0 and 1)

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
  pinMode(BUTTON_1, INPUT); // hours
  pinMode(BUTTON_2, INPUT); // minutes
  pinMode(BUTTON_3, INPUT); // activate or deactivate the ring 
  
  // Configure buzzer
  pinMode(BUZZER, OUTPUT); // Buzzer
  
  // RTC configuration 
  Wire.begin();
  rtc.begin();
  if (!rtc.isrunning()) {
    // If RTC not initialised with a time, set one from computer at build time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  time = rtc.now(); // Get the current time from RTC
}

/*********** Main loop **********/
void loop() {
  static unsigned long lastPush = millis(); // Store the last time button 1 or 2 was pushed
  static unsigned long lastNote = millis(); // Store the last time a note was played
  static unsigned long lastStopPush = 0;    // Store the last time button 3 was pushed
  static bool alarm = false;                // Alarm time is displayed
  static bool ringActive = false;           // Alarm is active (in order to ring when this is the right time)
  static bool alarmRinging = false;         // True the alarm is ringing

  // Increment hours every 200ms when pressed
  if (!digitalRead(BUTTON_1) && (millis() - lastPush) > 200) {
    alarmTime = alarmTime + 3600;
    lastPush = millis();
    alarm = true;
  }

  // Increment minutes every 200ms when pressed
  if (!digitalRead(BUTTON_2) && (millis() - lastPush) > 200) {
    alarmTime = alarmTime + 60;
    lastPush = millis();
    alarm = true;
  }

  if (!digitalRead(BUTTON_3)) {
    // If the alarm was ringing, a push on button 3 stops it
    if (alarmRinging) {
      alarmRinging = false;
    }
    
    // Check if the push lasts 3 seconds 
    if (lastStopPush == 0) {
      lastStopPush = millis();
    } else if ((millis() - lastStopPush) > 3000) {
      ringActive = !ringActive;
      lastStopPush = millis();
      tone(BUZZER, 880, 300);
    }
  }
  
  // When the button is released, reinit lastStopPush
  if (digitalRead(BUTTON_3) && lastStopPush != 0) {
      lastStopPush = 0;
  }

  // Update the time every minutes
  if (updateTime) {
    time = rtc.now();
    updateTime = false;
    // Check if it's time to start the alarm
    if (time.hour() == alarmTime.hour() && time.minute() == alarmTime.minute() && ringActive) {
      alarmRinging = true;
    }
  }
  
  // Generate the ring
  if (alarmRinging && (millis() - lastNote) > 600) {
    tone(BUZZER, 880, 300);
    lastNote = millis();
  }

  int minutes;
  int hours;
  // Displays the alarm time during 3 seconds
  if (alarm && (millis() - lastPush) < 3000) {
    minutes = alarmTime.minute();
    hours = alarmTime.hour();
  } else { 
    // Then, if the alarm time was changed, store the new alarm time in EEPROM  
    if (alarm) {
      EEPROM.write(0, alarmTime.hour());
      EEPROM.write(1, alarmTime.minute());
      alarm = false;
    }
    minutes = time.minute();
    hours = time.hour();
  }
  
  // Display the new time
  sevseg.setNumber((hours * 100) + minutes, ringActive ? 2 : -1);
  sevseg.refreshDisplay();
}

/*********** Functions **********/

// Used by the interruption every ms
void updateTimer() {
  static int milliseconds = 0;
  milliseconds++;
  // Every minute, it's time to update the time displayed
  if (!(milliseconds % 60000)) {
    updateTime = true;
    milliseconds = 0;
  }
}
