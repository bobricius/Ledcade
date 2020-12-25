Display::Display()
{
//	lc.shutdown(0, false);
//	lc.setIntensity(0, 10);
//	lc.clearDisplay(0);
clearMatrix();
}

void Display::newGame() 
{
clearMatrix();
chaos(500);
clearMatrix();
}

void Display::drawSprite(const unsigned char *sprite) 
{
  byte cc;
	for (int b = 0; b < 8; b++) 
	{
//		lc.setColumn(0, r, sprite[r]);
//  lc.setColumn(0, y, row);
  for(byte r = 0; r < 8; r++){
cc=pgm_read_byte(sprite+b);
    if (cc  & (1<<r) ) {  //sprite[b]
      leds[b][7-r] = 1;
    }
    else {  
      leds[b][7-r] = 0;
    }
  }

//    leds[y][x] = 1;
	}
}

void Display::drawRow(int y, byte row)
{
//	lc.setColumn(0, y, row);
  for(byte r = 0; r <8; r++){

    if ( row & (1<<r) ) {
      leds[y][r] = 1;
    }
    else {  
      leds[y][r] = 0;
    }
  }

//    leds[y][x] = 1;
}

void Display::update()
{
}

void Display::clear(int duration)
{
  clearMatrix();
	delay(duration);
}

void Display::clear()
{
  clearMatrix();
}


void Display::draw(Ball *ball)
{
	drawObject(1, ball->position);
}

void Display::draw(Controller *controller)
{
	drawObject(controller->size, controller->position);
}

void Display::drawPixel(int x, int y)
{
//	drawPixel(x, y, true);
    leds[y][x] = 1;
}

void Display::drawPixel(int x, int y, bool on)
{
	//lc.setLed(0, y, x, on);
//	lc.setLed(0, 7-x, y, on);
    leds[y][x] = on;
}

void Display::drawObject(int size, point position)
{
	for (int i = 0; i < size; i++)
	{
		//lc.setLed(0, position.y, position.x + i, true);
//		lc.setLed(0, 7 - (position.x + i), position.y, true);
    leds[position.y][position.x + i] = 1;
	}
}

void Display::flashSprite(const unsigned char   *sprite, int frames, int duration)
{
	for (int i = 0 ; i < frames; i++)
	{
		drawSprite(sprite);

		delay(duration/4);
    playTone( 30, 9000);
		clear(duration/4);
	}
  displayNumber(score);
  delay(1000);
}

void Display::chaos(int duration)
{
	clear();
    talkieBegin();
setupDisplayTimer();    
    drawSprite(n3_ico);
        delay(300);
talkieSay(spTHREE);
//    playTone( 100, 1000);
    drawSprite(n2_ico);
    delay(300);
talkieSay(spTWO);

//    playTone( 100, 2000);
    drawSprite(n1_ico);
    delay(300);
talkieSay(spONE);
//    playTone( 100, 3000);
    drawSprite(go_ico);
    delay(300);
talkieSay(spGO);
  clear();
  talkieEnd();
setupDisplayTimer();
}


// Interrupt routine
void update() {
  updateAudio();
  updateDisplay();
}

void updateAudio() {
  static const int jumpToneMaxSteps = 30;
  static int jumpToneStep = jumpToneMaxSteps;
  
  if(playJumpTone) {
//    Serial.println("Playing jump tone.");
    playJumpTone = false;
    jumpToneStep = 0;
  }
  
  if(jumpToneStep < jumpToneMaxSteps) {
    int frequency = ((int)(NOTE_A6 - NOTE_A3)
                      * ((double)jumpToneStep) / jumpToneMaxSteps) + NOTE_A3;
    toneWorkaround(speaker, frequency, 1);
    jumpToneStep++;
  }
}

void playDeathTone() {
  toneWorkaround(speaker, NOTE_FS1, 200);
  delay(20);
  toneWorkaround(speaker, NOTE_C1, 600);
}

// plays a tone without using the default library, which conflicts with FrequencyTimer2
void toneWorkaround(byte tonePin, int frequency, int duration) {
  int period = 1000000L / frequency;
  int pulse = period / 2;
  for (long i = 0; i < duration * 1000L; i += period) {
    digitalWrite(tonePin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(tonePin, LOW);
    delayMicroseconds(pulse);
  }
}

void explode(int duration) {

  for (int i = duration * 10; i > 0; i-=10 ) {
    digitalWrite(3, HIGH);
    delayMicroseconds(duration);
    digitalWrite(3, LOW);
    delayMicroseconds(random(1,300));
  }
}


void updateDisplay() {
  static byte col = 0;
  static long lastBlink = millis();
  
  // Turn whole previous column off
  digitalWrite(cols[col], LOW);
  
  col = (col + SIZE - 1) % SIZE;
  
  for(int row = 0; row < SIZE; row++) {
    // Turn this led ON or OFF
    if(leds[col][7 - row] == 1) {
      digitalWrite(rows[row],  LOW);
    } else {
      digitalWrite(rows[row], HIGH);
    }
  }
  // Turn whole column on at once (for equal lighting times)
  digitalWrite(cols[col], HIGH);
}


// ------------------- Setup methods -----------------------

void setupPins() {
  
  // matrix pins
 for (byte i = 0; i <= 17; i++) {
    pinMode(i, OUTPUT);
  }
}
 
void clearMatrix() {
  // Clear display array
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      leds[i][j] = 0;
    }
  }
}

void setupDisplayTimer() {

  Timer1.initialize(1925); // 8kHz sample rate
  Timer1.attachInterrupt(update); 
}

void displayNumber(int number)
{
//  byte data;
byte ir;
byte cd; 
byte n1;
byte n2;
    display.clear();
  for( ir = 0; ir <3; ir++){
n1=(pgm_read_byte(&font_data[(48+(number/10)*3)+ir]));
n2=(pgm_read_byte(&font_data[(48+(number % 10)*3)+ir]));
  for( cd = 0; cd <5; cd++){
    if ( n1 & (1<<cd) ) {
    leds[cd+1][ir] = 1;

    }
    if ( n2 & (1<<cd) ) {
    leds[cd+1][ir+4] = 1;
    }
  }
    

}
}

void displayRam (void)
{
     playTone( 100, 15000);
    displayNumber(FreeRam()/10);
    delay (2000);
}
int FreeRam(void)
{
  extern int  __bss_end;
  extern int  *__brkval;
  int free_memory;
  if((int)__brkval == 0) {
    // if no heap use from end of bss section
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  }
  else {
    // use from top of stack to heap
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}
