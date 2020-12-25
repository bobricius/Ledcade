#define ControllerSize 3
#define ControllerPin 2
#define ControllerRightButtonPin A5
#define ControllerLeftButtonPin A4
#include <LinkedList.h>
#include "LedControl.h"
#include "types.h"
#include "sprites.h"
#include "menu.h"
#include "controller.h"
#include "ball.h"
#include "display.h"
#include "pong.h"
#include "tedShow.h"
#include "SpaceInvaders.h"
#include "Snake.h"
#include "breakout.h"
#include "racing.h"
#include <TimerOne.h>
#include "font.h"
#include <avr/wdt.h>
#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers
#include <avr/pgmspace.h>
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC
#define speaker   3
/*************************************************
 * Public Constants
 *************************************************/
Pong pong;
Controller controller;
Display display;    
Menu menu;
TedShow tedShow;
SpaceInvaders spaceInvaders;
Snake snake;
BreakOut breakout;
Racing racing;
int score=0;
long gameDelay = 10;
long elapsed;
volatile byte wakeupActive=0;
byte forceSleep=0; // set to 1 to force sleep in next loop


//byte cols[8] = {9,6,12,8,1,13,0,15}; // black devkit
//byte rows[8] = {4,5,11,14,10,16,17,7};

byte rows[8] = { 15,11,8,0,6,17,13,12}; /// arcade
byte cols[8] = {7,10,5,9,16,4,14,1};

//byte rows[8] = { 12,13,17,6,0,8,11,15}; /// blue
//byte cols[8] = {1,14,4,16,9,5,10,7};

#define SIZE 8
static unsigned long lastTick = 0; // set up a local variable to hold the last time we moved forward one second
static unsigned long lastSleep = 0; // set up a local variable to hold the last time we moved forward one second

static unsigned long leftTick = 0; // set up a local variable to hold the last time we moved forward one second
static unsigned long rightTick = 0; // set up a local variable to hold the last time we moved forward one second

// the matrix state
byte leds[SIZE][SIZE];

// the currentscreen offset
int screenOffset;

// sound
boolean playJumpTone;

// death
//boolean doodlerAlive;
//long timeOfDeath;

// lighting modes
boolean blinking;
boolean invertedColors;

//typedef enum {
//  ACTIVE, INACTIVE
//} PlatformState;

//typedef struct {
//  int x;
//  int y;
//  
//  PlatformState state;
//} Platform;
//
//// the platforms, one per matrix row
//Platform platforms[SIZE];

  
void setup() 
{

	randomSeed(analogRead(0));
  setupPins();
  clearMatrix();
  setupDisplayTimer();
	menu.initialize();
  attachInterrupt(0, modeInt, FALLING); // FALLING CHANGE RISING
    lastSleep = millis();
//   displayRam();
    playTone( 50, 15000);
}

void loop() 
{
	controller.update();	

 if (((millis() - lastSleep) >= 20000)||(forceSleep==1)) {
    forceSleep=0;
    setupSleep(); 
    wakeupActive=0;
    display.drawSprite(off_ico);

    playTone( 100, 8000);
    playTone( 100, 7000);
    playTone( 100, 6000);    
    display.clear();
  Timer1.stop();
  Timer1.detachInterrupt();
    for(int row = 0; row < SIZE; row++) {
      digitalWrite(rows[row], HIGH);
  }
    while (wakeupActive==0){
    sleep_mode(); //Stop everything and go to sleep. Wake up if the Timer2 buffer overflows or if you hit the button
    playTone( 100, 6000);
    playTone( 100, 7000);
    playTone( 100, 8000);   
    }
      setupDisplayTimer();
    lastSleep = millis();
    // continue after INT button wakeup   

 }


	if (millis() - elapsed > gameDelay)
	{
		elapsed = millis();		

		menu.update();
	}
}

void playTone(long duration, int freq) {
// duration in mSecs, frequency in hertz

  duration *= 700;
  int period = (1.0 / freq) * 1000000;
  long elapsed_time = 0;
  int amax=0;
  byte up=1;

    while (elapsed_time < duration) {
      digitalWrite(speaker,HIGH);
      delayMicroseconds(period / 2);
      digitalWrite(speaker, LOW);
      delayMicroseconds(period / 2);
      elapsed_time += (period);  
  }  
}


EMPTY_INTERRUPT(WDT_vect);

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void system_sleep(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);

//  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_SAVE); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
//  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}

void setupSleep() {
    //Power down various bits of hardware to lower power usage  

  //Shut off ADC, TWI, SPI, Timer0, Timer1
//  ADCSRA &= ~(1<<ADEN); //Disable ADC
//  ACSR = (1<<ACD); //Disable the analog comparator
//  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
////  DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0

//  ACSR = (1<<ACD); //Disable the analog comparator
//  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
//  DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0

  adc_disable();
  power_twi_disable();
  power_spi_disable();
  power_usart0_disable(); //Needed for serial.print
  sleep_bod_disable();
//  power_timer0_disable(); //Needed for delay and millis()
//  power_timer1_disable();
  
///  set_sleep_mode(SLEEP_MODE_PWR_SAVE); //Stop everything and go to sleep. Wake up if the Timer2 buffer overflows or if you hit the button
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void modeInt()
{
  wakeupActive=1;
}
