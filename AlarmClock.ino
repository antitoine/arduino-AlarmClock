/* AlarmClock */ 

/*********** Includes **********/
#include "TimerOne.h"
#include "SevSeg.h"

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
#define BUZZER     3 // Pin conncted to the buzzer
#define LED        A3 // Pin connected to the LED

/*********** Notes **********/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

/*********** Melody **********/

// Mario main theme melody
int mainMelody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

// Mario main them tempo
int mainTempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

// Underworld melody
int underworldMelody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};

// Underwolrd tempo
int underworldTempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

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
  pinMode(BUZZER, OUTPUT); // Buzzer
  pinMode(LED, OUTPUT); // Led indicator when singing a note
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

void sing(int song) {
  int* melody = NULL;
  int melodySize = 0;
  int* tempo = NULL;

  switch (song) {
    case 2:
      melody = underworldMelody;
      melodySize = sizeof(underworldMelody) / sizeof(int);
      tempo = underworldTempo;
      break;
    case 1:
    default:
      melody = mainMelody;
      melodySize = sizeof(mainMelody) / sizeof(int);
      tempo = mainTempo;
      break;
  }
  for (int thisNote = 0; thisNote < melodySize; thisNote++) {
 
    // to calculate the note duration, take one second
    // divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / tempo[thisNote];

    long frequency = melody[thisNote];
    long length = noteDuration;
    digitalWrite(LED, HIGH);
    long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
    //// 1 second's worth of microseconds, divided by the frequency, then split in half since
    //// there are two phases to each cycle
    long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
    //// multiply frequency, which is really cycles per second, by the number of seconds to
    //// get the total number of cycles to produce
    for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
      digitalWrite(BUZZER, HIGH); // write the buzzer pin high to push out the diaphram
      delayMicroseconds(delayValue); // wait for the calculated delay value
      digitalWrite(BUZZER, LOW); // write the buzzer pin low to pull back the diaphram
      delayMicroseconds(delayValue); // wait again or the calculated delay value
    }
    digitalWrite(LED, LOW);

    // to distinguish the notes, set a minimum time between them
    delay(noteDuration);
  }
}
