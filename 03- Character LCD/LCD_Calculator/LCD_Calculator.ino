#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','*'},
  {'4','5','6','/'},
  {'7','8','9','+'},
  {'C','0','=','-'}
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

byte rowPins[ROWS] = {KEYPAD1, KEYPAD2, KEYPAD3, KEYPAD4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KEYPAD5, KEYPAD6, KEYPAD7, KEYPAD8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd = LiquidCrystal(RS, E, D4, D5, D6, D7);

String OPERATORS = "*/+-";
long op1 = 0l, op2 = 0l, result=0l;
char operation;
int state;

#define INIT 0
#define OP1_SET 1
#define OP_SET 2
#define OP2_SET 3
#define RESULT 4

void print_at_center(String message, int row){
  int col = (16 - message.length()) / 2;
  lcd.setCursor(col, row);
  lcd.print(message);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  print_at_center("Input:", 0);
}

long str_to_long(String number){
  long result = 0l;
  int start = number.charAt(0) == '-' ? 1 : 0;
  for(int i = start; i < number.length(); i++){
    result = result * 10 + number.charAt(i) - '0';
  }
  return number.charAt(0) == '-' ? -result : result;
}

String long_to_str(long number){
  if (number >= 0){
    return String(number);
  }
  else{
    return "-" + String(-number);
  }
}
String calculate(long op1, long op2, char operation){
  switch (operation){
    case '+':
      return long_to_str(op1 + op2);
    case '-':
      return long_to_str(op1 - op2);
    case '*':
      return long_to_str(op1 * op2);
    case '/':
      if (op2 == 0){
        return "DB0 ERROR!";
      }
      return long_to_str(op1 / op2);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.waitForKey();

  if (key >= '0' && key <= '9'){
    if (state == INIT || state == OP1_SET || state == RESULT){
      op1 = op1 * 10 + key - '0';
      if (state == RESULT){
        lcd.clear();
        print_at_center("Input:", 0);
      }
      print_at_center(String(op1), 1);
      state = OP1_SET;
    }
    else if (state == OP_SET || state == OP2_SET){
      op2 = op2 * 10 + key - '0';
      print_at_center(String(op1) + operation + String(op2), 1);
      state = OP2_SET;
    }
    
  }
  else if (OPERATORS.indexOf(key) != -1){
    
    if (state == OP1_SET || state == OP_SET){
      operation = key;
      state = OP_SET;
    }
    else if (state == OP2_SET){
      String result_string = calculate(op1, op2, operation);
      if (result_string == "DB0 ERROR!"){
        state = RESULT;
        op1 = 0;
        print_at_center(result_string, 1);
      }
      else{
        op1 = str_to_long(result_string);
        print_at_center(String(op1) + operation , 1);
      }
      
      operation = key;
      op2 = 0l;
      state = OP_SET;
    }
    else if (state == RESULT){  
      operation = key;
      op1 = result;
      lcd.clear();
      print_at_center("Input:", 0);
      state = OP_SET;
    }
    
    print_at_center(String(op1) + operation, 1);
    
  }
  else if (key=='=' && state == OP2_SET){
    String result_string = calculate(op1, op2, operation);
    Serial.println(result_string);
    if (result_string == "DB0 ERROR!"){
      result = 0l;
      print_at_center(result_string, 1);
    }
    else{
      result = str_to_long(result_string);
      print_at_center("Result: " + String(result), 1);
    }
    
    op1 = 0;
    op2 = 0;
    state = RESULT;
  }
  else if (key == 'C'){
    op1 = 0;
    op2 = 0;
    state = INIT;
    lcd.clear();
    print_at_center("Input:", 0);  
  }
}
