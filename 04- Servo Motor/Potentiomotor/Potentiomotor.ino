// Include the Servo library 
#include <Servo.h> 

// Declare the Servo pin 
int servoPin = 9;  
// Create a servo object 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Serial.begin(9600);
   Servo1.attach(servoPin);
   Servo1.write(0);
}


void loop() {
  // put your main code here, to run repeatedly:
  int input = analogRead(A5);
  input = map(input, 0, 1023, 0, 180);
  Servo1.write(input);
  Serial.println(input);
}
