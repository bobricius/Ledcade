


const int switch1 = A4;
const int switch2 = A5;


// ------------------- Constants and global variables -----------------------

// physics constants
const float gravity = -0.4;
const float jumpVelocity = 3;

// the maximum height before moving up on the screen
const int doodlerMaximumOnScreenY = 5;

// the matrix state
//byte leds[SIZE][SIZE];

// the currentscreen offset
//int screenOffset;

// sound
//boolean playJumpTone;

// death
boolean doodlerAlive;
long timeOfDeath;

// lighting modes
//boolean blinking;
//boolean invertedColors;

// the character
typedef struct {
  int x;
  int y;
  
  float velocityY;
} Doodler;

Doodler doodler;


typedef enum {
  ACTIVE, INACTIVE
} PlatformState;

typedef struct {
  int x;
  int y;
  
  PlatformState state;
} Platform;

// the platforms, one per matrix row
Platform platforms[SIZE];

// the index of the platform with smallest y
// the platforms array traversed circularly
int platformSmallestYIndex;

// y of the last platform shown
// used to know how long it's been 
// since the last active platform
int lastPlatformY;

// ------------------- Arduino callback -----------------------

//void setup() {
//  randomSeed(analogRead(A6));
//  
//  setupPins();
//  clearMatrix();
//  setupInitialVariables();
//  renderMatrix();
//  setupDisplayTimer();
//  
//  pinMode(switch1, INPUT_PULLUP);
//  pinMode(switch2,  INPUT_PULLUP);
//}


//void loop() {
//  if(doodlerAlive) {
//    updateXPosition();
//    updateYPosition();
//    renderMatrix();
//    checkForDeath();
//  } else {
//    waitForSwitch();
//  }
//  
//  delay(75);
//}

// ------------------- Game logic -----------------------

void checkForDeath() {
  if(doodler.y < screenOffset) {

    doodlerAlive = false;
    display.flashSprite(lost, 4, 500);
    setupInitialVariables();
    renderMatrix();
    invertedColors = false;
  }
}

void waitForSwitch() { 
  if(digitalRead(switch1) == LOW || digitalRead(switch2) == LOW) {

      doodlerAlive = true;
  }
}

void updateXPosition() {    
  if(digitalRead(switch1) == LOW) {
//      Serial.println("Move left.");
      doodler.x = (doodler.x + SIZE - 1) % SIZE;
      lastSleep = millis();
  }
  
  if(digitalRead(switch2) == LOW) {
//      Serial.println("Move right.");
      doodler.x = (doodler.x + 1) % SIZE;
      lastSleep = millis();
  }
}

void updateYPosition() {
  doodler.velocityY += gravity;
  doodler.y += doodler.velocityY;
  
  // if doodler is going down and has floor
  if(doodler.velocityY <= 0 && doodlerHasFloor()) {
    playJumpTone = true;
    doodler.velocityY = jumpVelocity;
 score++;
  }
  
  // if screen should scroll up
  if(doodler.y - screenOffset > doodlerMaximumOnScreenY) {
    screenOffset++;
    updatePlatforms();
  }
}

void updatePlatforms() {
  if(platforms[platformSmallestYIndex].y < screenOffset) {
    platforms[platformSmallestYIndex].y += SIZE;
    
    if(platforms[platformSmallestYIndex].y - lastPlatformY > 4
        || (platforms[platformSmallestYIndex].y - lastPlatformY > 2 &&
            random(3) == 0)) {
      // 5th inactive in a row OR ( 33% chance AND not two inactives before it )
      platforms[platformSmallestYIndex].x = random(SIZE - 1);
      platforms[platformSmallestYIndex].state = ACTIVE;
      lastPlatformY = platforms[platformSmallestYIndex].y;
    } else {
      platforms[platformSmallestYIndex].state = INACTIVE;
    }
    
    platformSmallestYIndex = (platformSmallestYIndex + 1) % SIZE;
  }
}

boolean doodlerHasFloor() {
  for(int i = 0; i < SIZE; i++) {
    Platform p = platforms[i];
    
    if(p.state == ACTIVE &&
        p.y - screenOffset >= 0 &&
        (doodler.y == p.y || doodler.y == p.y + 1) &&
        (doodler.x == p.x || doodler.x == p.x + 1)) {
      explode(50);
       return true;
    }
  }
  
  return false;
}

// ------------------- Actions -----------------------
 
void renderMatrix() {
  clearMatrix();
  
  // the doodler y position
  int y = indexForY(doodler.y);
  
  if(validPoint(doodler.x, y)) {
    leds[y][doodler.x] = 1;
  }
  
  if(validPoint(doodler.x, y + 1)) {
    leds[y + 1][doodler.x] = 1;
  }
  
  for(int i = 0; i < SIZE; i++) {
    Platform p = platforms[i];
    
    y = indexForY(p.y);
    
    // if the platform should be displayed
    if(p.state == ACTIVE){
     
      if(validPoint(p.x, y)) {
        leds[y][p.x] = 1;
      }
     
       if(validPoint(p.x + 1, y)) {
         leds[y][p.x + 1] = 1;
       }
    }
  }
}

boolean validPoint(int x, int y) {
 return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}
 
int indexForY(int y) {
  return SIZE - 1 - (y - screenOffset);
}
 
// Interrupt routine
//void update() {
//  updateAudio();
//  updateDisplay();
//}

//void updateAudio() {
//  static const int jumpToneMaxSteps = 25;
//  static int jumpToneStep = jumpToneMaxSteps;
//  
//  if(playJumpTone) {
////    Serial.println("Playing jump tone.");
//    playJumpTone = false;
//    jumpToneStep = 0;
//  }
//  
//  if(jumpToneStep < jumpToneMaxSteps) {
//    int frequency = ((int)(NOTE_A6 - NOTE_A3)
//                      * ((double)jumpToneStep) / jumpToneMaxSteps) + NOTE_A3;
//    toneWorkaround(speaker, frequency, 1);
//    jumpToneStep++;
//  }
//}

//void playDeathTone() {
//  toneWorkaround(speaker, NOTE_FS1, 200);
//  delay(20);
//  toneWorkaround(speaker, NOTE_C1, 600);
//}

// plays a tone without using the default library, which conflicts with FrequencyTimer2
//void toneWorkaround(byte tonePin, int frequency, int duration) {
//  int period = 1000000L / frequency;
//  int pulse = period / 2;
//  for (long i = 0; i < duration * 1000L; i += period) {
//    digitalWrite(tonePin, HIGH);
//    delayMicroseconds(pulse);
//    digitalWrite(tonePin, LOW);
//    delayMicroseconds(pulse);
//  }
//}

//void updateDisplay() {
//  static byte col = 0;
//  static long lastBlink = millis();
//  
//  if(blinking && millis() - lastBlink > 350) {
//    invertedColors = !invertedColors;
//    lastBlink = millis();
//  }
//  
//  // Turn whole previous column off
//  digitalWrite(cols[col], LOW);
//  
//  col = (col + SIZE - 1) % SIZE;
//  
//  for(int row = 0; row < SIZE; row++) {
//    // Turn this led ON or OFF
//    if(leds[col][7 - row] == 1) {
//      digitalWrite(rows[row], invertedColors ?  HIGH : LOW);
//    } else {
//      digitalWrite(rows[row], invertedColors ?  LOW : HIGH);
//    }
//  }
//  // Turn whole column on at once (for equal lighting times)
//  digitalWrite(cols[col], HIGH);
//}

//void showSkull() {
//  int skull[SIZE][SIZE] = SKULL;
//
//  for (int i = 0; i < SIZE; i++) {
//    for (int j = 0; j < SIZE; j++) {
//      leds[i][j] = skull[i][j];
//    }
//  }
//}

// ------------------- Setup methods -----------------------

//void setupPins() {
//  // switch pins
//  pinMode(switch1, INPUT);
//  pinMode(switch2, INPUT);
//  
//  // speaker pin
//  pinMode(speaker, OUTPUT);
//  
//  // matrix pins
// for (byte i = 0; i <= 17; i++) {
//    pinMode(i, OUTPUT);
//  }
//}
 
//void clearMatrix() {
//  // Clear display array
//  for (int i = 0; i < SIZE; i++) {
//    for (int j = 0; j < SIZE; j++) {
//      leds[i][j] = 0;
//    }
//  }
//}

//void setupDisplayTimer() {
//  // Turn off toggling of pin 11
//  FrequencyTimer2::disable();
//  // Set refresh rate (interrupt timeout period)
//  FrequencyTimer2::setPeriod(2000);
//  // Set interrupt routine to be called
//  FrequencyTimer2::setOnOverflow(update);
//}

void setupInitialVariables() {
  blinking = false;
  platformSmallestYIndex = 0;
  
  // initial position
  doodler.x = SIZE / 2 - 1;
  doodler.y = 4;
  doodler.velocityY = 0;
  
  screenOffset = 0;
  
  for(int i = 0; i < SIZE; i++) {
    Platform p;
    p.x = random(SIZE - 1);
    p.y = i;
    p.state = INACTIVE;
    platforms[i] = p;
  }
  
  platforms[0].x = SIZE / 2 - 2;
  platforms[0].state = ACTIVE;
  
  platforms[3].x = 5;
  platforms[3].state = ACTIVE;

  platforms[7].x = 0;
  platforms[7].state = ACTIVE;
  
  lastPlatformY = 7;
}
