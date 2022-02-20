#include <Adafruit_NeoPixel.h>

#define DRIVER_nAUTON 7
#define ENABLE 6
#define STRIP 5

#define SECOND 1000

#define START_FLASH_TIME 2 * SECOND
#define END_FLASH_TIME 3 * SECOND

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

bool starting = true;

unsigned long effectTime;
unsigned long startTime;

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

void changeHappened(){
  effectTime = startTime = millis();
  starting = true;
}

unsigned long getEffectTime(){
  return(millis() - effectTime);
}

unsigned long getSectionTime(){
  return(millis() - startTime);
}

void makeDriver() {
  changeHappened();
  ledSetAll(0, 255, 0);
  state = DRIVER;
}

void makeScoring(){
  ledSetAll(0, 0, 0);
  changeHappened();
  state = SCORING;
}

void makeSkills(){
  changeHappened();
  state = ROBOT_SKILLS;
}

void makeAuto(){
  changeHappened();
  state = AUTO;
  ledSetAll(0, 255, 0);
}

void makeEndMatch(){
  changeHappened();
  state = END_MATCH;
  ledSetAll(255, 131, 0);
}

void makeIdle(){
  changeHappened();
  ledSetAll(255, 255, 255);
  state = IDLE;
}

void confusedDefault(){
  makeIdle();
}

void stateEndMatch(){
  if(getSectionTime() > END_FLASH_TIME){
    makeIdle();
  }
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
  if(starting && getSectionTime() > START_FLASH_TIME) {
    starting = false;
    ledSetHalves(255, 0, 0, 0, 0, 255);
  }
  if(!enabled){
    makeEndMatch();
  }
}

void stateAuto(){
  if(starting && getSectionTime() > START_FLASH_TIME) {
    starting = false;
    ledSetAll(50, 50, 255);
  }

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

  ledSetAll(0, 0, 0);
  delay(1000);

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

  delay(20);
}