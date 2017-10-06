/*****************************************************************
 * 15-348 Spring 2016 Assignment 3 Task 1 By Sharjeel Khan       *
 ****************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "lcd.h"

/* Make sure not to write to lcd until person has let go of the button */
void Debounce(byte index) {
  byte value = PORTA & 0x0F;
  while(value == index){
    value = PORTA & 0x0F;
  }
}

void main(void) {
   
  int pulse = 0;
 
  EnableInterrupts
 
  SPI_Initialize();
  LCD_Initialize();
 
  /* Set port A lower bits to input and higher bits to output*/
  DDRA = 0xF0;
 
  /* Make port B have no output */
  PORTA = 0x0F;
   
  for(;;) {
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
              LCD_SendCharacter('1');
              break;
            case 5:
              LCD_SendCharacter('2');
              break;
            case 6:
              LCD_SendCharacter('3');
              break;
            case 7:
              LCD_SendCharacter('A');
              break;
            default:
              break;
          }
          /* Do not write to lcd until button released */
          Debounce(0xE);    
          break;
          
        /* Button press was in row 2 */
        case 0xD:
          switch(i) {
            case 4:
              LCD_SendCharacter('4');
              break;
            case 5:
              LCD_SendCharacter('5');
              break;
            case 6:
              LCD_SendCharacter('6');
              break;
            case 7:
              LCD_SendCharacter('B');
              break;
            default:
              break;
          }
          /* Do not write to lcd until button released */
          Debounce(0xD);    
          break;
          
        /* Button press was in row 3 */
        case 0xB:
          switch(i) {
            case 4:
              LCD_SendCharacter('7');
              break;
            case 5:
              LCD_SendCharacter('8');
              break;
            case 6:
              LCD_SendCharacter('9');
              break;
            case 7:
              LCD_SendCharacter('C');
              break;
            default:
              break;
          }
          /* Do not write to lcd until button released */
          Debounce(0xB);  
          break;
          
        /* Button press was in row 4 */
        case 0x7:
          switch(i) {
            case 4:
              LCD_SendCharacter('*');
              break;
            case 5:
              LCD_SendCharacter('0');
              break;
            case 6:
              LCD_SendCharacter('#');
              break;
            case 7:
              LCD_SendCharacter('D'); 
              break;
            default:
              break;
          }
          /* Do not write to lcd until button released */
          Debounce(0x7);    
          break;
        default:
          break;
      }
    }
  }
}