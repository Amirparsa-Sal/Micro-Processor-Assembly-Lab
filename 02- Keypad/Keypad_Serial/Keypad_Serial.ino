#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

#define LED1 5
#define LED2 6
#define LED3 7
#define LED4 8
#define LED5 9
#define LED6 10
#define LED7 11
#define LED8 12
#define LED9 13

#define KEYPAD1 A5
#define KEYPAD2 A4
#define KEYPAD3 A3
#define KEYPAD4 A2
#define KEYPAD5 A1
#define KEYPAD6 A0
#define KEYPAD7 2
#define KEYPAD8 3

byte rowPins[ROWS] = {KEYPAD1, KEYPAD2, KEYPAD3, KEYPAD4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KEYPAD5, KEYPAD6, KEYPAD7, KEYPAD8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Getting the selected key
  char key = keypad.getKey();

  // Print the key
  if (key != NO_KEY){
    Serial.println(key);
  }
}
