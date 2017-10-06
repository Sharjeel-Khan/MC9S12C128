#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "lcd.h"

int cursor = 0;
int line = 1;
int previouskey = -1;
int currentkey = -1;
int period = 1000;
char sendString[9];
int pulse = 0;
int stage = 0;
byte bit = 0;

void interrupt 8 TC0handler() {
  // ack the interrupt
  TFLG1 |= 0x01;

  // setup the next interrupt to be after period number of clock ticks 
  TC0 = TCNT + period;
  
  if(pulse == 0 && stage < 4) {
    stage = stage + 1;
    if(stage == 4) {
      PORTB_BIT0 = bit;  
    } 
    else {
      PORTB_BIT0 = 1;
      pulse = 1;
    } 
  }
  else if (pulse == 1 && stage < 4) {
    stage = stage + 1;
    if(stage == 4) {
      PORTB_BIT0 = bit;  
    } 
    else {
      PORTB_BIT0 = 0;
      pulse = 0;
    } 
  } 
  else {
    PORTB_BIT0 = bit;
    stage = stage + 1;  
  }

}

const struct state{
  char output[9];
  const struct state* next[9];
};

typedef const struct state stype;

/* State Machine */
stype FSM[28] = { 
  {{'.','A','D','G','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*0*/
  {{',','A','D','G','J','M','P','T','W'},{&FSM[2],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*1*/
  {{'"','A','D','G','J','M','P','T','W'},{&FSM[3],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*2*/
  {{'1','A','D','G','J','M','P','T','W'},{&FSM[0],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*3*/
  {{'.','B','D','G','J','M','P','T','W'},{&FSM[1],&FSM[5],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*4*/
  {{'.','C','D','G','J','M','P','T','W'},{&FSM[1],&FSM[6],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*5*/
  {{'.','2','D','G','J','M','P','T','W'},{&FSM[1],&FSM[0],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*6*/  
  {{'.','A','E','G','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[8],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*7*/
  {{'.','A','F','G','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[9],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*8*/
  {{'.','A','3','G','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[0],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*9*/
  {{'.','A','D','H','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[11],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*10*/
  {{'.','A','D','I','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[12],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*11*/
  {{'.','A','D','4','J','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[0],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*12*/
  {{'.','A','D','G','K','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[14],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*13*/
  {{'.','A','D','G','L','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[15],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*14*/
  {{'.','A','D','G','5','M','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[0],&FSM[16],&FSM[19],&FSM[22],&FSM[25]}}, /*15*/
  {{'.','A','D','G','J','N','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[17],&FSM[19],&FSM[22],&FSM[25]}}, /*16*/
  {{'.','A','D','G','J','O','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[18],&FSM[19],&FSM[22],&FSM[25]}}, /*17*/
  {{'.','A','D','G','J','6','P','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[0],&FSM[19],&FSM[22],&FSM[25]}}, /*18*/
  {{'.','A','D','G','J','M','R','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[20],&FSM[22],&FSM[25]}}, /*19*/
  {{'.','A','D','G','J','M','S','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[21],&FSM[22],&FSM[25]}}, /*20*/ 
  {{'.','A','D','G','J','M','7','T','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[0],&FSM[22],&FSM[25]}}, /*21*/
  {{'.','A','D','G','J','M','P','U','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[23],&FSM[25]}}, /*22*/
  {{'.','A','D','G','J','M','P','V','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[24],&FSM[25]}}, /*23*/
  {{'.','A','D','G','J','M','P','8','W'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[0],&FSM[25]}}, /*24*/
  {{'.','A','D','G','J','M','P','T','X'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[26]}}, /*25*/
  {{'.','A','D','G','J','M','P','T','Y'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[27]}}, /*26*/
  {{'.','A','D','G','J','M','P','T','9'},{&FSM[1],&FSM[4],&FSM[7],&FSM[10],&FSM[13],&FSM[16],&FSM[19],&FSM[22],&FSM[0]}}, /*27*/     
};

/* Make sure not to write to lcd until person has let go of the button */
void Debounce(byte index) {
  byte value = PORTA & 0x0F;
  while(value == index){
    value = PORTA & 0x0F;
  }
}

/* Setup output capture */
void SetupOC() {
  //setup timer channel 0 for OC
  TIOS  = TIOS | 0x01;
  
  // Enable the timer
  TSCR1 = 0x80;
  
  // Enable interrupts for Output Compare at channel 0.
  TIE  |= 0x01;
  
  // Specify that we will be using channel 0 for output compare. 
  // Note that each channel can be either configured as output compare or 
  // input capture (we have not studied this concept yet)
  TFLG1 |= 0x01;
  
  // Set the Data register for Channel 0 to be after 2000 clock ticks
  // This mainly means that the channel 0 interrupt will be called
  // after TCNT counter reaches the value TCNT+period
  TC0 = TCNT + period;
}

/* Disable output capture */
void DisableOC() {
  //setup timer channel 0 for OC
  TIOS  = TIOS & 0xFE;
  
  // Disable the timer
  TSCR1 = 0x00;
  
  // Disable interrupts for Output Compare at channel 0.
  TIE  &= 0xFE;
  
  // Specify that we will be using channel 0 for output compare. 
  // Note that each channel can be either configured as output compare or 
  // input capture (we have not studied this concept yet)
  TFLG1 &= 0xFE;
}

/* Transmit a byte */
void SendByte(byte input){
   int number_of_ones = 0;
   int prev_stage = 3;
   int shift = 0;
   pulse = 1;
   stage = 0;
   period = 1000;
   
   PORTB_BIT0 = 1;

   /* Setup output capture */
   SetupOC();
   
   /* Synchronize */
   for(; stage < 4;) {
      if(stage == 3) {
        bit = input & 0x01;
        period = 10000;
      }
   }
    
   if(bit == 1){
     number_of_ones = number_of_ones + 1; 
   }
   
   /* Send character and parity bit */
   for(; stage < 13; ) {
      /* Get ith bit for the character */
      if(stage != prev_stage && stage != 11) {
        prev_stage = stage;
        shift = 8 - (11 - stage);
        bit = (input >> shift) & 0x01;
        
        /* Count number of ones */
        if(bit == 1){
          number_of_ones = number_of_ones + 1; 
        }
         
      } 
      /* Transmit the parity bit */
      else if (stage != prev_stage) {
        bit = number_of_ones & 0x01;
      }
   }
   
   /* Disable output capture */
   DisableOC();
   PORTB_BIT0 = 0;  
}


/* Transmit a String */
void SendString(char* text){
  int i = 0;
  
  /* Read each character and transmit it */
  for(;text[i] != '\0' && i < 8;){
    SendByte(text[i]);
    i = i + 1;    
  }
  
  /* Transmit null terminator */
  SendByte('\0');
}


int read_bit() {
  int i = 0;
  int j = 0;
  byte type = 0;
  
  
  for(i = 4; i < 8; i++) {
    switch(i) {
      /* Enable keypad row 8 */
      case 4:
        PORTA = 0x7F;
        break;
      /* Enable keypad row 7 */
      case 5:
        PORTA = 0xBF;
        break;
      /* Enable keypad row 6 */
      case 6:
        PORTA = 0xDF;
        break;
      /* Enable keypad row 5 */
      case 7:
        PORTA = 0xEF;
        break;
      default:
        break;
    }
    
    /* Get the keypad button press */
    type = PORTA & 0x0F;
    
    switch(type) {
      /* Button press was in row 1*/
      case 0xE:   
        switch(i) {
          /* 1 button is pressed */
          case 4:
            j = 0;
            break;
          /* 2 button is pressed */
          case 5:
            j = 1;
            break;
          /* 3 button is pressed */
          case 6:
            j = 2;
            break;
          default:
            break;
        }
        Debounce(0xE);
        return j;
        
      /* Button press was in row 2 */
      case 0xD:
        switch(i) {
          /* 4 button is pressed */
          case 4:
            j = 3;
            break;
          /* 5 button is pressed */
          case 5:
            j = 4;
            break;
          /* 6 button is pressed */
          case 6:
            j = 5;
            break;
          default:
            break;
        }
        Debounce(0xD);
        return j;
        
      /* Button press was in row 3 */
      case 0xB:
        switch(i) {
          /* 7 button is pressed */
          case 4:
            j = 6;
            break;
          /* 8 button is pressed */
          case 5:
            j = 7;
            break;
          /* 9 button is pressed */
          case 6:
            j = 8;
            break;
          default:
            break;
        }
        Debounce(0xB);
        return j;
      
      /* Button press was in row 4 */
      case 0x7:
        switch(i) {
          /* # button is pressed */
          case 6:
            j = 10;
            break;
          default:
            break;
        }
        Debounce(0x7);    
        return j;
        
      default:
        break;
    }
    
  }
  return 9;
}



void main(void) {
  /* put your own code here */
  volatile int r = 9;
  stype *cstate = &FSM[0];
  unsigned int i = 0;
  unsigned int j = 0;
	EnableInterrupts;
	
	SPI_Initialize();
  LCD_Initialize();

  DDRA = 0xF0;
  DDRB = 0x01;
 
  /* Make port B have no output */
  PORTA = 0x0F;
  
  PORTB_BIT0 = 0;
   
  for(;;) {  
    r = 9;
    
    /* Read bit or wait for a second */
    for (i = 0; i < 20 && r==9; i++)
    {
      for (j = 0; j < 328 && r==9; j++)
      {
        r = read_bit();
      }
    } 
    
    j = 0;
    
    /* If key pressed and waited for a second, move to next cursor */
    if(i == 20 && currentkey != -1) {
      cursor += 1;
      LCD_CursorPos(line,cursor);
      
      cstate = cstate->next[0];
      currentkey = -1;  
    }

    if (r != 9 && r != 10) {
      previouskey = currentkey;
      currentkey = r;
      
      /* If key pressed and different from previous key, move to next cursor */
      if(previouskey != -1 && previouskey != currentkey){
        cursor += 1;
        LCD_CursorPos(line,cursor);      
      }
      
      /* Show character on the screen */
      LCD_SendCharacter(cstate->output[r]);
      sendString[cursor] = cstate->output[r];
      LCD_CursorPos(line,cursor);
      
      /* Move to next state */
      cstate = cstate->next[r];
       
    } 
    else if (r == 10) {
      /* Add Null Terminator to String*/
      sendString[cursor] = '\0';
      cursor += 1;
      
      /* Transmit string */
      SendString(sendString);
      
      /* Reinitialize everything to original state */
      cursor = 0;
      previouskey = -1;
      currentkey = -1;
      LCD_SendString(1,"        ");
      LCD_CursorPos(line,cursor);
      cstate = &FSM[0];
      
      Delay(1000);
    }
    
  }
}
