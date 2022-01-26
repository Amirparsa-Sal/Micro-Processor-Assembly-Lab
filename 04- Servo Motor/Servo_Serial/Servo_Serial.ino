// Include the Servo library 
#include <Servo.h> 

int current_degree = 180;

// Declare the Servo pin 
int servoPin = 9;  
// Create a servo object 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Serial.begin(9600);
   Servo1.attach(servoPin);
   Servo1.write(180); 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Enter the number: ");
   while(Serial.available() == 0) {
   }
  String input = Serial.readString();
  long number = input.toInt();
  Serial.println(number);
  int diff = -number;
  int final_degree = current_degree + diff;
  if (final_degree < 0 || final_degree > 180){
    Serial.println("Can not rotate");
  }
  else{
    current_degree += diff;
    Serial.print("Moved to ");
    Serial.println(current_degree);
    Servo1.write(current_degree);
  }
}
