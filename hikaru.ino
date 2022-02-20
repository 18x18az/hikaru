#include <Adafruit_NeoPixel.h>

#define DRIVER_nAUTON 7
#define ENABLE 6
#define STRIP 5

#define PER_SEGMENT 1
#define NUM_SEGMENTS 2
#define NUM_PIXELS PER_SEGMENT * NUM_SEGMENTS
#define PER_SIDE NUM_PIXELS / 2

#define BRIGHTNESS 25

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

void ledSetAll(uint8_t r, uint8_t g, uint8_t b){
  for(uint16_t i = 0; i < NUM_PIXELS; i++){
    pixels.setPixelColor(i, r, g, b);
  }
  pixels.show();
}

void ledSetHalves(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2){
  for(uint16_t i = 0; i < PER_SIDE; i++){
    pixels.setPixelColor(i, r1, g1, b1);
  }
  for(uint16_t i = PER_SIDE; i < NUM_PIXELS; i++){
    pixels.setPixelColor(i, r2, g2, b2);
  }
  pixels.show();
}

void makeDriver() {
  Serial.println("Entering driver control");
  state = DRIVER;
}

void makeScoring(){
  Serial.println("Scoring auto");
  state = SCORING;
}

void makeSkills(){
  state = ROBOT_SKILLS;
}

void makeAuto(){
  state = AUTO;
}

void makeEndMatch(){
  state = END_MATCH;
}

void makeIdle(){
  ledSetAll(255, 255, 255);
  state = IDLE;
}

void confusedDefault(){
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
  pixels.setBrightness(BRIGHTNESS);
  makeIdle();
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