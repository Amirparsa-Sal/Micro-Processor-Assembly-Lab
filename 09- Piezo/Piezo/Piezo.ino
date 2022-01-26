#include "pitches.h"
#include "themes.h"
#include <Keypad.h>

#define PIEZO_PIN 10

#define NOTE_BASE_TIME 1000

const byte ROWS = 4; 
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

#define KEYPAD1 9
#define KEYPAD2 8
#define KEYPAD3 7
#define KEYPAD4 6
#define KEYPAD5 5
#define KEYPAD6 4
#define KEYPAD7 3
#define KEYPAD8 2

byte rowPins[ROWS] = {KEYPAD1, KEYPAD2, KEYPAD3, KEYPAD4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KEYPAD5, KEYPAD6, KEYPAD7, KEYPAD8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void play_music(int melody[], int note_duration[], int melody_length);

void setup() {

  // iterate over the notes of the jingleBellsMelody:
}

void loop() {
  char c = keypad.waitForKey();
  if (c == '1'){
    play_music(jingleBellsMelody, jingleBellsNoteDurations, sizeof(jingleBellsMelody)/sizeof(int));
  }
  else if (c == '2'){
    play_music(odeToJoyMelody, odeToJoyNoteDurations, sizeof(odeToJoyMelody)/sizeof(int));
  }
  else if (c == '3'){
    play_music(underworldMelody, underworldNoteDurations, sizeof(underworldMelody)/sizeof(int));
  }
  // no need to repeat the jingleBellsMelody.
}

void play_music(int melody[], int note_duration[], int melody_length){
  
  for (int thisNote= 0; thisNote< melody_length; thisNote++) {

    // to calculate the NOTE_duration, take NOTE_BASE_TIME divided by the NOTE_type.

    //e.g. quarter NOTE_= NOTE_BASE_TIME / 4, eighth NOTE_= NOTE_BASE_TIME/8, etc.

    int noteDuration = NOTE_BASE_TIME / note_duration[thisNote];

    tone(PIEZO_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:

    noTone(PIEZO_PIN);
  }
  
}
