#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <IRremote.h>

// Pins
#define SS_PIN 10
#define RST_PIN 2
#define L_MOTOR_FORWARD_PIN 7
#define L_MOTOR_BACKWARD_PIN 6
#define R_MOTOR_FORWARD_PIN 9
#define R_MOTOR_BACKWARD_PIN 8
#define RIGHT_SENSOR_PIN A0
#define MIDDLE_SENSOR_PIN A1
#define LEFT_SENSOR_PIN A3
#define BUZZER_PIN 5
#define IR_RC_PIN A2

// Constants
// hash: dd44d3498faedc7fd28126168a3ca7a5426b7fc7
const String TAG_UID = "93 58 1C 16";
const String SECRET = "0000";
const int RC_NUMBER_BUTTONS[10] = {26775, 12495, 6375, 31365, 4335, -7632, 23205, 17085, 641, 21165};
const int RC_RESERVE_NUMBER_BUTTONS[10] = {26775, 12495, 6375, 31365, 4335, 14535, 23205, 17085, 19125, 21165};
const int RC2_RESERVE_NUMBER_BUTTONS[10] = {26775, 12495, 6375, 31365, 4335, 14535, 23205, 17085, 31448, 21165};
const int RC_PLUS100_BUTTON = -26521;
const int RC_PLUS200_BUTTON = -20401;
const int RC_PLUS_BUTTON = -22441;
const int RC_MINUS_BUTTON = -8161;
const int RC_EQ_BUTTON = -28561;
const int RC_PLAY_BUTTON = -15811;
const int RC_CH_BUTTON = -23971;

// Modules
LiquidCrystal_I2C lcd(0x27, 16, 2);
IRrecv irrecv(IR_RC_PIN);   
decode_results results; 
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

/*
 * States:
 * 0) INIT_STATE: first and initial state -> motors off LCD off. check if we can disable ir sensors.
 * if remote_eq is pressed it goes to WAIT_FOR_TAG_AUTH state. 
 * 
 * 1) WAIT_FOR_AUTH_STATE: lcd on and print "Show me the tag\n or enter password!". motors off. 
 * if the password or tag is not correct the buzzer will be triggered and the lcd prints dont touch my car. if the password or tag is correct it goes to ASK_FOR_MODE state. 
 * password is entered using remote control. user can use numbers and then press the remote_eq button to confirm.
 * 
 * 2) ASK_FOR_MODE_STATE: lcd print "please choose a mode.\n +100 for free mode. +200 for line follower mode. motors off.
 * if +100 is pressed it goes to FREE_MODE_STATE else LINE_FOLLOWER_STANDBY_STATE.
 * 
 * 3) LINE_FOLLOWER_STANDBY_STATE: lcd print "press remote_eq to start.". motors off.
 * 
 * 4) FREE_MODE_STATE: motors off initially.
 * if remote_2 is pressed both motors will move forward.
 * if remote_4 is pressed left motor will stop and right motor will continue moving to its direction.
 * if remote_6 is pressed right motor will stop and left motor will continue moving to its direction.
 * if remote_8 is pressed both motors will move backwards.
 * if remote_5 is pressed the car will stop and all direction will be reset.
 * 
 * 5) LINE_FOLLOWER_STATE: motors off initially.
 * if remote_eq is pressed the robot will start follwing a black line.
 * 
 * Note 1: In all states use CH to go to init state.
 * Note 2: If a button is pressed the buzzer must be triggered. 
 * 
 */
const byte INIT_STATE = 0;
const byte WAIT_FOR_AUTH_STATE = 1;
const byte ASK_FOR_MODE_STATE = 2;
const byte FREE_MODE_STATE = 3;
const byte LINE_FOLLOWER_STANDBY_STATE = 4;
const byte LINE_FOLLOWER_MODE_STATE = 5;

// A function to check whether an input value is related to a RC number button.
char getNumberButton(int value){
  for(byte i = 0; i < 10; i++){
    if (value == RC_NUMBER_BUTTONS[i] || value == RC_RESERVE_NUMBER_BUTTONS[i] || value == RC2_RESERVE_NUMBER_BUTTONS[i]){
      return i + '0';
    }
  }
  return 0;
}

String read_uid(MFRC522 mfrc522){
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}


void print_at_center(String message, byte row){
  int col = (16 - message.length()) / 2;
  lcd.setCursor(col, row);
  lcd.print(message);
}

bool check_password(String password){
  if (password == SECRET){
    return true;
  }
  return false;
}

// set default state to INIT.
byte state = INIT_STATE;
String password = "";
String pass_stars = "";

byte dir[2] = {0, 0};

void move_robot(){
  if (dir[0] == 0){
    digitalWrite(L_MOTOR_FORWARD_PIN, 0);
    digitalWrite(R_MOTOR_FORWARD_PIN, 0);  
    digitalWrite(L_MOTOR_BACKWARD_PIN, 0);
    digitalWrite(R_MOTOR_BACKWARD_PIN, 0); 
    return; 
  }
  if (dir[0] == 1){
    digitalWrite(L_MOTOR_BACKWARD_PIN, 0);
    digitalWrite(R_MOTOR_BACKWARD_PIN, 0); 
    if (dir[1] == 1){
      digitalWrite(L_MOTOR_FORWARD_PIN, 1);
      digitalWrite(R_MOTOR_FORWARD_PIN, 0);  
    }
    else if (dir[1] == 0){
      digitalWrite(L_MOTOR_FORWARD_PIN, 1);
      digitalWrite(R_MOTOR_FORWARD_PIN, 1);  
    }
    else{
      digitalWrite(L_MOTOR_FORWARD_PIN, 0);
      digitalWrite(R_MOTOR_FORWARD_PIN, 1);  
    }
  }
  else{
    digitalWrite(L_MOTOR_FORWARD_PIN, 0);
    digitalWrite(R_MOTOR_FORWARD_PIN, 0);   
    if (dir[1] == 1){
      digitalWrite(L_MOTOR_BACKWARD_PIN, 1);
      digitalWrite(R_MOTOR_BACKWARD_PIN, 0); 
    }
    else if (dir[1] == 0){
      digitalWrite(L_MOTOR_BACKWARD_PIN, 1);
      digitalWrite(R_MOTOR_BACKWARD_PIN, 1); 
    }
    else{
      digitalWrite(L_MOTOR_BACKWARD_PIN, 0);
      digitalWrite(R_MOTOR_BACKWARD_PIN, 1); 
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(L_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(L_MOTOR_BACKWARD_PIN, OUTPUT);
  pinMode(R_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(R_MOTOR_BACKWARD_PIN, OUTPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(MIDDLE_SENSOR_PIN, INPUT);
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  lcd.begin(); // Init LCD
  lcd.display();
  lcd.noBacklight(); // Set diaplay to be turned off

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // Init IR sensors
  irrecv.enableIRIn();
}

void loop() {
//  Remote Control
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.     
  {     
   // Trigger the buzzer
   digitalWrite(BUZZER_PIN, HIGH);
   // Do the work in terms of the state
   char c;
   int value = results.value; // Results of decoding are stored in result.value  
   // Use CH button as power button
   if (value == RC_CH_BUTTON){
    state = INIT_STATE;
    lcd.clear();
    lcd.noBacklight();
    password= "";
    pass_stars = "";
    dir[0] = 0;
    dir[1] = 0;
    digitalWrite(L_MOTOR_FORWARD_PIN, 0);
    digitalWrite(R_MOTOR_FORWARD_PIN, 0);  
    digitalWrite(L_MOTOR_BACKWARD_PIN, 0);
    digitalWrite(R_MOTOR_BACKWARD_PIN, 0);
    digitalWrite(BUZZER_PIN, LOW);  
   }
   // Handle EQ button
   else if (value == RC_EQ_BUTTON){
    switch(state){
      case INIT_STATE:
        state = WAIT_FOR_AUTH_STATE;
        lcd.backlight();
        lcd.clear();
        print_at_center("Show me the tag", 0);
        print_at_center("or enter secret!", 1);
        break;
      case LINE_FOLLOWER_STANDBY_STATE:
        state = LINE_FOLLOWER_MODE_STATE;
        lcd.clear();
        print_at_center("Have fun :)", 0);
        break;
      case WAIT_FOR_AUTH_STATE:
        bool valid = check_password(password);
        if (valid){
          state = ASK_FOR_MODE_STATE;
          lcd.clear();
          print_at_center("Choose a mode!", 0);
          print_at_center("+100free+200flw", 1);
        }
        else{
          lcd.clear();
          print_at_center("Incorrect again:", 0);
        }
        password = "";
        pass_stars= "";
        break;
      default:
        break;
    }
   }
   else if (value == RC_PLUS100_BUTTON and state == ASK_FOR_MODE_STATE){
    dir[0] = 0;
    dir[1] = 0;
    state = FREE_MODE_STATE;
    lcd.clear();
    print_at_center("Buttons to move:", 0);
    print_at_center("2, 4, 5, 6, 8", 1);
   }
   else if (value == RC_PLUS200_BUTTON and state == ASK_FOR_MODE_STATE){
    dir[0] = 0;
    dir[1] = 0;
    state = LINE_FOLLOWER_STANDBY_STATE;
    lcd.clear();
    print_at_center("Line flwer mode!", 0);
    print_at_center("EQ to start!", 1);
   }
   else if(c = getNumberButton(value)){
    if (state == FREE_MODE_STATE){
      switch (c){
        case '2':
          dir[0] = 1;
          break;
        case '4':
          dir[1] = -1;
          break;
        case '5':
          dir[0] = 0;
          dir[1]=  0;
          break;
        case '6':
          dir[1] = 1;
          break;
        case '8':
          dir[0] = -1;
          break;
        default:
          break;
      }
      move_robot();
      lcd.clear();
      String power = "power: ";
      power += dir[0];
      String dir = "dicrection: ";
      dir = dir[1] == 1 ? dir + 'R' : dir[1] == 0 ? dir + '-' : dir + 'L';
      print_at_center(power, 0);
      print_at_center(dir, 1);
    }
    else if (state == WAIT_FOR_AUTH_STATE){
      password += c;
      pass_stars += '*';
      lcd.clear();
      print_at_center("Password:", 0);
      print_at_center(pass_stars, 1);
    }
   } 
   digitalWrite(BUZZER_PIN, LOW);
   irrecv.resume(); // Restart the ISR state machine and Receive the next value   
  }
  if (state == LINE_FOLLOWER_MODE_STATE){
    bool left_sensor = digitalRead(LEFT_SENSOR_PIN);
    bool middle_sensor = digitalRead(MIDDLE_SENSOR_PIN);
    bool right_sensor = digitalRead(RIGHT_SENSOR_PIN);
    if (left_sensor && middle_sensor && right_sensor){
      dir[0] = 0;
    }
    else if (!left_sensor && middle_sensor && right_sensor){
      dir[0] = 1;
      dir[1] = 1;
    }
    else if (left_sensor && middle_sensor && !right_sensor){
      dir[0] = 1;
      dir[1] = -1;
    }
    else if (!left_sensor && !middle_sensor && right_sensor){
      dir[0] = 1;
      dir[1] = 1;
    }
    else if (!left_sensor && middle_sensor && !right_sensor){
      dir[0] = 1;
      dir[1] = 0;
    }
    else if (left_sensor && !middle_sensor && !right_sensor){
      dir[0] = 1;
      dir[1] = -1;
    }
    else if (!left_sensor && !middle_sensor && !right_sensor){
      dir[0] = 1;
    }
    move_robot();
  }

  else if(state == WAIT_FOR_AUTH_STATE){
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      String content= read_uid(mfrc522);
      if (content.equals(TAG_UID)){
        state = ASK_FOR_MODE_STATE;
        lcd.clear();
        print_at_center("Choose a mode!", 0);
        print_at_center("+100free+200flw", 1);
      }
      else{
        digitalWrite(BUZZER_PIN,HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN,LOW);
      }
    } 
  }
}
