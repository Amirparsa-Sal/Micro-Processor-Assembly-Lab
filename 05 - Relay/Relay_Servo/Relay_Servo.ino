#define RELAY_PIN 5

int output = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  output = 1 - output;
  digitalWrite(RELAY_PIN,output);
  Serial.println(output);
  delay(2000);
}
