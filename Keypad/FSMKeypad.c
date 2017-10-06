/*****************************************************************
 * 15-348 Spring 2016 Assignment 3 Task 2 By Sharjeel Khan       *
 ****************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "lcd.h"

int line = 1;
int cursor = 0;
int previouskey = -1;
int currentkey = -1;

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
          case 4:
            j = 0;
            break;
          case 5:
            j = 1;
            break;
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
          case 4:
            j = 3;
            break;
          case 5:
            j = 4;
            break;
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
          case 4:
            j = 6;
            break;
          case 5:
            j = 7;
            break;
          case 6:
            j = 8;
            break;
          default:
            break;
        }
        Debounce(0xB);
        return j;
        
      default:
        break;
    }
    
  }
  return 9;
}

void main(void) {
   
  volatile int r = 9;
  stype *cstate = &FSM[0];
  unsigned int i = 0;
  unsigned int j = 0;
 
  EnableInterrupts
 
  SPI_Initialize();
  LCD_Initialize();
 
  /* Set port A lower bits to input and higher bits to output*/
  DDRA = 0xF0;
 
  /* Make port B have no output */
  PORTA = 0x0F;
   
  for(;;) {
    r = 9;
    
    for (i = 0; i < 20 && r==9; i++)
    {
      for (j = 0; j < 328 && r==9; j++)
      {
        r = read_bit();
      }
    }
    
    /* If its been more than a second, move the cursor */
    j = 0;
    if(i == 20 && currentkey != -1) {
      cursor += 1;
      LCD_CursorPos(line,cursor);
      
      if(cursor == 8) {
        line = !line;
        cursor = -1;
      }
      
      cstate = cstate->next[0];
      currentkey = -1;  
    }

    if (r != 9) {
      previouskey = currentkey;
      currentkey = r;
      
      /* Check if previous button is different from current button then move cursor */
      if(previouskey != -1 && previouskey != currentkey){
        cursor += 1;
        LCD_CursorPos(line,cursor);      
      }
      
      /* If cursor moves out of screen, move to next line */
      if(cursor == 8) {
        line = !line;
        cursor = -1;
      }
      
      /* Write character and come back to current cursor */
      LCD_SendCharacter(cstate->output[r]);
      LCD_CursorPos(line,cursor);
      
      /* Change state to next state */
      cstate = cstate->next[r];  
    }
  }
}