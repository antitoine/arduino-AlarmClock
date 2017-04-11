#include "TimerOne.h"
#include "SevSeg.h"
SevSeg sevseg;

//display pins
int segA = 2;
int segB = 3;
int segC = 4;
int segD = 5;
int segE = 6;
int segF = 7;
int segG = 8;
int segP = 9;
int d1 = 10;
int d2 = 11;
int d3 = 12;
int d4 = 13;

int time = 0;

void setup() {
  // Set up segments
  byte numDigits = 4;
  byte digitPins[] = {d1, d2, d3, d4};
  byte segmentPins[] = {segA, segB, segC, segD, segE, segF, segG, segP};
  bool resistorsOnSegments = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool updateWithDelays = false;
  bool leadingZeros = false;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);
  
  // Init timer
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(incrementTimer);
}

void loop() {
  int minutes = (time / 60) * 100;
  int seconds = time % 60;
  int displayTime = minutes + seconds;
  sevseg.setNumber(displayTime, 2);
  sevseg.refreshDisplay();
}

void incrementTimer() {
  time++;
}
