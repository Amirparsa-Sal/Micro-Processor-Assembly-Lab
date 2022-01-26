#include <LiquidCrystal.h>

#define D4 11
#define D5 10
#define D6 9
#define D7 8
#define RS 13
#define E 12

LiquidCrystal lcd = LiquidCrystal(RS, E, D4, D5, D6, D7);

int x = 0, y = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.print('A');
}

void loop(){
  lcd.clear();
  y = 1 - y;
  x += 1;
  x = x % 16;
  lcd.setCursor(x, y);
  lcd.print('A');
  delay(1000);
}
