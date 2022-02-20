#include <Adafruit_NeoPixel.h>

#define DRIVER_nAUTON 7
#define ENABLE 6
#define STRIP 5

#define PER_SEGMENT 1
#define NUM_SEGMENTS 2
#define NUM_PIXELS PER_SEGMENT * NUM_SEGMENTS
#define PER_SIDE NUM_PIXELS / 2

Adafruit_NeoPixel pixels(NUM_PIXELS, STRIP, NEO_GRB + NEO_KHZ800);

enum FIELD_STATE {
  IDLE,
  AUTO,
  SCORING,
  DRIVER,
  ROBOT_SKILLS,
  END_MATCH
};

int driver;
int enabled;

int state = IDLE;

void makeDriver() {
  Serial.println("Entering driver control");
  state = DRIVER;
}

void makeScoring(){
  Serial.println("Scoring auto");
  state = SCORING;
}

void makeSkills(){
  Serial.println("Skills started");
  state = ROBOT_SKILLS;
}

void makeAuto(){
  Serial.println("Entering auto");
  state = AUTO;
}

void makeEndMatch(){
  Serial.println("Match ended");
  state = END_MATCH;
}

void makeIdle(){
  Serial.println("Returning to idle");
  state = IDLE;
}

void confusedDefault(){
  Serial.println("No idea what just happened");
  makeIdle();
}

void stateEndMatch(){
  makeIdle();
}

void stateSkills(){
  if(!ENABLE){
    makeEndMatch();
  }
}

void stateIdle(){
  if(enabled){
    if(driver){
      makeSkills();
    } else {
      makeAuto();
    }
  }
}

void stateDriver(){
  if(!enabled){
    makeEndMatch();
  }
}

void stateAuto(){
  if(!enabled){
    makeScoring();
  }
}

void stateScoring(){
  if(enabled){
    if(driver){
      makeDriver();
    } else {
      confusedDefault();
    }
  }
}

void setup(){
  pinMode(DRIVER_nAUTON, INPUT);
  pinMode(ENABLE, INPUT);
  pixels.begin();
  pixels.setBrightness(50);
  Serial.begin(9600);
  Serial.println("Hello world");
  pixels.setPixelColor(0, 0, 255, 0);
  pixels.setPixelColor(1, 0, 0, 255);
  pixels.show();
}

void loop() {
  driver = digitalRead(DRIVER_nAUTON);
  enabled = digitalRead(ENABLE);

  switch(state) {
      case IDLE:
        stateIdle();
        break;
      case AUTO:
        stateAuto();
        break;
      case DRIVER:
        stateDriver();
        break;
      case SCORING:
        stateScoring();
        break;
      case ROBOT_SKILLS:
        stateSkills();
        break;
      case END_MATCH:
        stateEndMatch();
        break;
  }

  delay(10);
}