#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "FastLED.h"

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 4; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 5; // Connects to module's TX

#define BLUE_RX 2
#define BLUE_TX 3
#define NUM_LEDS 300
#define DATA_PIN 9

SoftwareSerial myMp3(PIN_MP3_RX, PIN_MP3_TX);
SoftwareSerial myBlue(BLUE_RX, BLUE_TX); // RX | TX

CRGB leds[NUM_LEDS];
// Create the Player object
DFRobotDFPlayerMini player;
byte randomLED = 0;
byte ledChange = 0;
byte ledPower = 0;
byte redSet = 0;
byte greenSet = 0;
byte blueSet = 0;
byte red = 255;
byte green = 255;
byte blue = 255;
byte playing = 1;

void setup() {

  // Init USB serial port for debugging
    Serial.begin(9600);
  // Init serial port for DFPlayer Mini
  myMp3.begin(9600);
  myBlue.begin(9600);
  player.begin(myMp3);
  player.enableLoopAll();
  player.volume(2);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  if (myBlue.available()) {
    String command = myBlue.readString();
    if (command.equals("st")){
      player.start();
      playing = 1;
    }
    else if (command.equals("ps")){
      player.pause();
      playing = 0;
    }
    else if (command.equals("ne")){
      player.next();
    }
    else if (command.equals("pr")){
      player.previous();
    }
    else if (command.equals("vu")){
      player.volumeUp();
    }
    else if (command.equals("vd")){
      player.volumeDown();
    }
    else if (command.equals("pn")){
      ledChange = ledPower == 0;
      ledPower = 1;
    }
    else if (command.equals("pf")){
      ledChange = ledPower == 1;
      ledPower = 0;
    }
    else if (command.equals("rn")){
      ledChange = randomLED == 0;
      randomLED = 1;
    }
    else if (command.equals("rf")){
      ledChange = randomLED == 1;
      randomLED = 0;
    }
    else if (command.equals("gp")){
      String s = String(playing);
      s.concat(".");
      Serial.println(s);
      myBlue.print(s);
    }
    else if (command.equals("gc")){
      char buffer[10];
      sprintf(buffer, "%03d%03d%03d.", red, green, blue);
      myBlue.print(buffer);
    }
    else if (command.startsWith("c")){
      ledChange = 1;
      red = command.substring(1,4).toInt();
      green = command.substring(4,7).toInt();
      blue = command.substring(7,10).toInt();
    }

    if (!command.startsWith("g")){
      myBlue.print("1.");
    }
  }
  
  if (ledChange){
    if (ledPower) {
      if (randomLED){
        int red_random = random(256);
        int green_random = random(256);
        int blue_random = random(256);
        int i = random(146);
        leds[i] = CRGB(red_random, green_random, blue_random);
      }
      else{
        for(int i = 0; i < 145; i++){
          leds[i] = CRGB(red,green,blue);
        }
        ledChange = 0;
      }
    }
    else {
      FastLED.clear();
      ledChange = 0;
    }
    FastLED.show();
  }
  delay(50);
}
