#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 10
#define LED5 9
#define LED6 8
#define BUTTON1 2
#define BUTTON2 3

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(BUTTON1)){
  // Blink simultaneously
    for(int i = 0; i < strlen("Amirparsa") - 1; i++){
      for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, HIGH);
      }
      delay(500);
      for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, LOW);
      }
      delay(500);
    }

    for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, HIGH);
    }
  }
  else if (!digitalRead(BUTTON2)){
    // Turn of all LEDs
    for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, LOW);
      }
  }
}
