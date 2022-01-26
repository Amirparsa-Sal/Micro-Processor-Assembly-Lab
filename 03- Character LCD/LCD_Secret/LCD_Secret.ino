#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

#define D4 11
#define D5 10
#define D6 9
#define D7 8
#define RS 13
#define E 12

#define KEYPAD1 A5
#define KEYPAD2 A4
#define KEYPAD3 A3
#define KEYPAD4 A2
#define KEYPAD5 A1
#define KEYPAD6 A0
#define KEYPAD7 2
#define KEYPAD8 3

#define SECRET String("9831034")

byte rowPins[ROWS] = {KEYPAD1, KEYPAD2, KEYPAD3, KEYPAD4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KEYPAD5, KEYPAD6, KEYPAD7, KEYPAD8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd = LiquidCrystal(RS, E, D4, D5, D6, D7);

String password = "";

void print_at_center(String message, int row){
  int col = (16 - message.length()) / 2;
  lcd.setCursor(col, row);
  lcd.print(message);
  Serial.print("hello");
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  print_at_center("ENTER THE SECRET", 0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.waitForKey();
  if (key >= '0' && key <= '9'){
    password += key;
    print_at_center(password, 1);    
  }
  else if (key == '*'){
    lcd.clear();
    if (SECRET == password){
      print_at_center("CORRECT PASSWORD!", 0);
    }
    else{
      print_at_center("WRONG PASSWORD!", 0);
    }
    
    delay(2000);
    lcd.clear();
    print_at_center("ENTER THE SECRET", 0);
    password = "";
  }
}
