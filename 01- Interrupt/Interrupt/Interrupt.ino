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

  attachInterrupt(digitalPinToInterrupt(BUTTON1), blink_simultaneosly, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), turn_off, FALLING);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void turn_off(){
  for(int i=LED6; i<=LED1; i++){
    digitalWrite(i,LOW);  
  }  
}

void blink_simultaneosly(){
   for(int i = 0; i < strlen("Amirparsa") - 1; i++){
      for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, HIGH);
      }
      delay(60000);
      for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, LOW);
      }
      delay(60000);
    }

    for(int i = LED1; i >= LED6; i--){
        digitalWrite(i, HIGH);
    }
}
