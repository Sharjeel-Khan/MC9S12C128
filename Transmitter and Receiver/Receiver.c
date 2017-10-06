#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "lcd.h"

int line = 1;
char recvString[9];
int stage = -1;
unsigned int left = 0;
unsigned int right = 0;
unsigned int ending = 0;
int period = 0;
int readbyte = 0;
int done = 0;
byte parity = 0;
int numberofones = 0;
byte bit = 0;

/* Setup return */
void SetupRx() {
  //setup timer channel 0 for OC
  TIOS  &= ~0x01;
  
  // Enable the timer
  TSCR1 = 0x80;
  
  // Enable interrupts for Output Compare at channel 0.
  TIE  |= 0x01;
  
  TCTL4 = (TCTL4 & 0xFC) | 0x01;
  
  // Specify that we will be using channel 0 for output compare. 
  // Note that each channel can be either configured as output compare or 
  // input capture (we have not studied this concept yet)
  TFLG1 |= 0x01; 
}

/* Disable Return */
void DisableRx() {
  //setup timer channel 0 for OC
  TIOS  = TIOS | 0x01;
  
  // Enable the timer
  TSCR1 = 0x00;
  
  // Enable interrupts for Output Compare at channel 0.
  TIE  &= 0xFE;
  
  // Specify that we will be using channel 0 for output compare. 
  // Note that each channel can be either configured as output compare or 
  // input capture (we have not studied this concept yet)
  TFLG1 &= 0xFE;
  
  TCTL4 &= 0xFE;
}

/* Setup output capture */
void SetupOC() {
  //setup timer channel 0 for OC
  TIOS  = TIOS | 0x02;
  
  // Enable the timer
  TSCR1 = 0x80;
  
  // Enable interrupts for Output Compare at channel 0.
  TIE  |= 0x02;
  
  // Specify that we will be using channel 0 for output compare. 
  // Note that each channel can be either configured as output compare or 
  // input capture (we have not studied this concept yet)
  TFLG1 |= 0x02;
  
  // Set the Data register for Channel 0 to be after 2000 clock ticks
  // This mainly means that the channel 0 interrupt will be called
  // after TCNT counter reaches the value TCNT+period
  TC1 = TCNT + 1000;
}

/* Disable output capture */
void DisableOC() {
  //setup timer channel 0 for OC
  TIOS  = TIOS & 0xFD;
  
  // Disable the timer
  TSCR1 = 0x00;
  
  // Disable interrupts for Output Compare at channel 0.
  TIE  &= 0xFD;
  
  // Specify that we will be using channel 0 for output compare. 
  // Note that each channel can be either configured as output compare or 
  // input capture (we have not studied this concept yet)
  TFLG1 &= 0xFD;
}

/* Handler for Input Capture */
void interrupt 8 TC0handler() {
  // ack the interrupt
  TFLG1 |= 0x01; 
  
  // Set next time interrupt
  right = TCNT;
  TC1 = TCNT + 10000;
  
  // Get bit
  bit = PTT & 0x01;
  
  // See the first rising edge of SYNC
  if(stage == -1) {
    TCTL4 = (TCTL4 & 0xFC) | 0x3;
    stage += 1;
    left = TCNT;  
  } 
  
  // SYNC Stage
  else if (stage < 4) {
		stage += 1;
    bit = PTT & 0x01;
    
    // Calculate width of the stage
  	if(right > left) {
  	  period = right - left;
  	  left = right;
  	}
  	else {
  	  ending = 65535 - left;
  	  period = ending + right;
  	  left = right;
  	}
  	
  	// Check bit is correct at particular stage  
    if(stage%2==!bit) {
    
      // Check if width of signal is between 10%
      if(period <= 900 || period >= 1100){
        stage = -1;
        TCTL4 = (TCTL4 & 0xFC) | 0x1;
      }
    }
  	else {
  		stage = -1;
  		TCTL4 = (TCTL4 & 0xFC) | 0x1;
  	}
  } 
  // Byte and Parity Stage
  else if(stage < 13) {
    stage += 1;
  
    // Calculate width of the stage
  	if(right > left) {
  	  period = right - left;
  	  left = right;
  	}
  	else {
  	  ending = 65535 - left;
  	  period = ending + right;
  	  left = right;
  	}
  	
  	// Check if period is between 10%
  	if(period <= 9000 || period >= 11000){
  	  stage = -1;
  	  TCTL4 = (TCTL4 & 0xFC) | 0x1;
  	  DisableOC();
  	  period = 0;
  	  readbyte = 0;
  	  parity = 0;
  	  numberofones = 0;
  	}
  }
}

/* Handler for Output Compare */
void interrupt 9 TC1handler() {
  left = TCNT;
	TC1 = TCNT + 10000;
	TFLG1 |= 0x02;
	stage += 1;
}

/* Receive a String */
void RecvString(){
	int index = 0;
	int prevstage = -1;
	volatile int stop = 1;
	stage = -1;
	prevstage = stage;
 	done = 0;
  period = 0;
  readbyte = 0;
  parity = 0;
  numberofones = 0;
  SetupRx();
	
	
	/* Read each character one at a time */
	for(index = 0; index < 9 && stop; index++) {
	 
  	
  	/* Wait until the character is read */
  	for(;done==0;) {
  	  /* If stage is changed, read bit */
  	  if(prevstage != stage && stage != -1) {
  	    prevstage = stage;
  	    
  	    /* After SYNC, setup ouput compare */
  	    if(stage == 3){
  	      SetupOC();
  	    }
  	    /* Get byte */ 
  	    else if(stage >= 4 && stage < 13) {   
  	      bit = PTT & 0x01;
    	    readbyte |= (bit << (stage - 4));

    	    /* Count number of ones in byte */
    	    if(bit && stage == 12) {
    	      numberofones += 1;
    	    }
    		
    		  /* Check if parity bit is right */
    	    if(stage == 12 && bit!=numberofones % 2) {
    	      done = 2;
    	    }
    	    else if (stage == 12) {
    		    done = 1; 
    	    }
  	    }
  	  }
  	}
  	
  	/* Disable IC and OC */
  	prevstage = -1;
  	period = 0;
  	parity = 0;
  	numberofones = 0;
  	stage = -1;
  	DisableOC();
  	DisableRx();
  	SetupRx();
  	
  	/* If byte is corrupted, give back zero */
  	if(done != 2){
  	  recvString[index] = (char)(readbyte & 0xFF);
  	  
  	  if(recvString[index] == '\0'){
  	    stop = 0;  
  	  }
  	  done = 0;
  	  readbyte = 0;
  	} else {
  	  recvString[index] = ' ';
  	  done = 0;
  	  readbyte = 0;  
  	}
  	

	}
}

void main(void) {
  /* put your own code here */
  EnableInterrupts;
	
  SPI_Initialize();
  LCD_Initialize();

  DDRA = 0xF0;
  DDRT &= ~0x01;
 
  /* Make port B have no output */
  PORTA = 0x0F;
  recvString[8] = '\0';
  
  for(;;) {  
    RecvString();
    LCD_SendString(1, recvString);  
  }
}
