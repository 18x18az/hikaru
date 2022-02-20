#define DRIVER_nAUTON 7
#define ENABLE 6

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
  Serial.begin(9600);
  Serial.println("Hello world");
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