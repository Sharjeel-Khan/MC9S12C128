/*****************************************************************
 * 15-348 Spring 2016 Assignment 5 By Sharjeel Khan              *
 * Initial Code was taken from the book on pages 564 and editted * 
 * for the assignment                                            *
 ****************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "lcd.h"

volatile byte highbyte;
volatile int n = 0;
volatile int period = 75;
volatile int state =0;

// Sin Wave
const unsigned char sinWave[32] = {
  120,140,158,176,191,203,212,218,220,218,212,
  203,191,176,158,140,120,100,82,64,49,37,28,
  22,20,22,28,37,49,64,82,100
};

/* Setup output capture */
void SetupOC(int p) {
  //setup timer channel 0 for OC
  TIOS  = TIOS | 0x02;
  
  // Enable the timer
  TSCR1 = 0x80;
  
  // Enable interrupts for Output Compare at channel 0.
  TIE  |= 0x02;
  
  // Specify that we will be using channel 0 for output compare. 
  TFLG1 |= 0x02;
  
  // Set the Data register for Channel 0 to be after 50 clock ticks 
  // to start right away
  TC1 = TCNT + 50;
  PACTL = 0;
  
  period = p;
}

/* Handler for Analog to Digital Interrupt */
void interrupt 22 handleAtoD(void){
  ATDSTAT0 |= 0x80;  
  if(ATDSTAT1 & 0x01){
    highbyte = ATDDR0H;   
  }
}

/* Handler for Output Compare */
void interrupt 9 TC1handler() {
  /* Acknowledge interrupt and set next interrupt */
  TFLG1 |= 0x02;
	TC1 = TCNT + period;
  
  /* Change value only if not very far away */	
	if(state != 0) {
	  PWMDTY0 = sinWave[n];
	  if(n < 31){
	    n = n + 1;  
	  } else {
	    n = 0;  
	  }
	}
}

/* Setup Pulse Width Modulation */
void PWM_Init(void){
  DDRP |= 0x01;
  PWMPOL |= 0x01 ;
  PWMCLK &= ~0x01;
  PWMPRCLK &= ~0x07;
  PWMCAE &= ~0x01;
  PWMCTL &= ~0x7C;
  PWMPER0 = 255;
  PWMDTY0 = 0;  
}

/* Setup Analog to Digital */
void ATD_Init(void){
  ATDCTL2 = 0x82;
  ATDCTL3 = 0x20;
  ATDCTL4 = 0x81;
  ATDCTL5 = 0x30;  
}

void main(void) {
  /* put your own code here */
  EnableInterrupts;
	
  /* Setup Interrupts */	
  ATD_Init();
  PWM_Init();
  SetupOC(75);
  
  /* Setup Port B and PWM */
  DDRB = 0x07;
  PORTB = 0x0;
  PWME |= 0x01;  
  for(;;) { 
	/* Based on how far, set color state */
    if(highbyte < 60) {
      state = 0;
    } 
    else if(highbyte < 130) {
      state = 1;
    } 
    else {
      state = 2;
    }
    
	/* Based on color state, make noise or not */	
    if(state == 0){ 
      PORTB = 0x04;
      Delay(40);
    } 
    else if (state == 1) {      
      PORTB = 0x02;
      Delay(30);
      state = 0;
      Delay(30);
    } 
    else {
      PORTB = 0x01;
      Delay(10);
      state = 0;
      Delay(10);  
    }
  }
}
