// Include the Servo library 
#include <Servo.h> 
#include <Keypad.h>


const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

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

int degree = 0;

// Declare the Servo pin 
int servoPin = 9;  
// Create a servo object 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Serial.begin(9600);
   Servo1.attach(servoPin); 
}
void loop(){ 
   Serial.print("Please enter the degree: ");
   char key = keypad.waitForKey();
   // Make servo go to 0 degrees 
   if (key >= '0' && key <= '9'){
    degree = degree * 10 + key - '0';
    Serial.println(degree);
   }
   else if (key == '*'){
    degree = 0;
   }
   else if (key == '#'){
    Serial.println("Done");
    if (degree <= 180){
      degree = 180 - degree;
      Servo1.write(0);
    }
    else{
      degree = 360 - degree;
      Servo1.write(180);
    }
    delay(1000);
    Servo1.write(degree);
    degree = 0; 
   }
}
