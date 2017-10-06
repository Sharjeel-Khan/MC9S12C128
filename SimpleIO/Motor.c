#include <hidef.h>      /* common defines and macros */
#include <mc9s12c128.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"

#define LOW1 440
#define LOW2 470
#define LOW3 487
#define HIGH1 520
#define HIGH2 540
#define HIGH3 580

/* 15-358 Spring 2016
 * By Sharjeel Khan */

/* Code taken from the course webpage */
void Delay(unsigned int time)
{
 // function delays "time" amount of milliseconds

 unsigned int i;
 unsigned int j;

 for (i = 0; i < time; i++)
 {
  for (j = 0; j < 328; j++)
  {
  }
 }
}

void main(void) {
   
   EnableInterrupts;
   
   /* Set port B to output*/
   DDRB = 0x1F;
   
   /* Set port A to input*/
   DDRA = 0x00;
   
   /* Make port B have no output */
   PORTB = 0x00;
   
   for(;;) {
      unsigned int j, intensity;
      byte b;      
      
      /* If PB1 is pressed, pulse pattern 1 should start */
      if(PORTA_BIT0==0) {
        /* Wait until release to begin pulse pattern 1 */
        for(;PORTA_BIT0==0;);
        
        /*Loop until PB2 is pressed*/ 
        for(b=0x01; PORTA_BIT1!=0 && PORTA_BIT3!=0 && PORTA_BIT4!=0;b<<=1) {
          /* If you are at LED4, go back to LED1*/
          if(b == 0x10)
            b = 0x01;
          PORTB = b;
          
          Delay(200);
          
          /* If PB2 is pressed, wait until it is pressed again and continue pattern */
          if(PORTA_BIT2==0) {
            for(;PORTA_BIT2==0;);
            for(;PORTA_BIT2!=0;);
            for(;PORTA_BIT2==0;);
          }
        }
      } 
      /* If PB2 is pressed, pulse pattern 2 should start */
      else if (PORTA_BIT1 == 0) {
        /* Wait until release to begin pulse pattern 2 */
        for(;PORTA_BIT0==0;);
        
        /*Loop until PB1 is pressed*/
        for(b=0x08; PORTA_BIT0!=0 && PORTA_BIT3!=0 && PORTA_BIT4!=0;b>>=1) {  
          /* If you are at LED1, go back to LED4*/
          if(b == 0x00)
            b = 0x08;
          PORTB = b;
          
          Delay(200);
          
          /* If PB2 is pressed, wait until it is pressed again and continue pattern */
          if(PORTA_BIT2==0) {
            for(;PORTA_BIT2==0;);
            for(;PORTA_BIT2!=0;);
            for(;PORTA_BIT2==0;);
          }
        }
      }
      
      /* If PB3 is pressed, begin motor to spin clockwise */
      if(PORTA_BIT3 == 0) {
        b = PORTB;
        intensity = 0;
        
        /* Wait until release to begin motor to spin clockwise */
        for(;PORTA_BIT3==0;);

        /*Loop until PB4 is pressed*/
        for(;PORTA_BIT4 != 0;) {

          PORTB = 0x10;
          
          /* Increase speed when PB3 is pressed again*/
          if(intensity == 0){
            for (j = 0; j < LOW3; j++);
          } else if (intensity == 1) {
            for (j = 0; j < LOW2; j++);
          } else {
            for (j = 0; j < LOW1; j++);
          }
          
          PORTB = b;
          
          Delay(20);
          
          /* Increase speed when PB3 is pressed again*/
          if(PORTA_BIT3==0) 
          {
            intensity += 1;
            for(;PORTA_BIT3==0;);
          }
         
        }

        /* Wait until release to move to next command */
        for(;PORTA_BIT4==0;);
      } 
      /* If PB4 is pressed, begin motor to spin anticlockwise */
      else if (PORTA_BIT4 == 0) {
        b = PORTB;
        intensity = 0;
        /* Wait until release to begin motor to spin anticlockwise */
        for(;PORTA_BIT4==0;);
        
        /*Loop until PB3 is pressed*/
        for(;PORTA_BIT3 != 0;) {
          PORTB = 0x10;
          
          /* Increase speed when PB4 is pressed again*/
          if(intensity == 0){
            for (j = 0; j < HIGH1; j++);
          } else if (intensity == 1) {
            for (j = 0; j < HIGH2; j++);
          } else {
            for (j = 0; j < HIGH3; j++);
          };
          
          PORTB = b;
          
          Delay(20);
          
          /* Wait until release to move to next command */
          if(PORTA_BIT4==0) 
          {
            intensity += 1;
            for(;PORTA_BIT4==0;);
          }
          
        }
        
        for(;PORTA_BIT3==0;);
      }
   }
}