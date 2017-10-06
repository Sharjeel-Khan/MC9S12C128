;*****************************************************************
;* 15-348 Spring 2016 Assignment Task 5 By Sharjeel Khan         *
;* Initial Code was taken from lab1.asm from course webpage      *
;* and editted for the assignment
;*****************************************************************

; export symbols
            XDEF Entry               ; export 'Entry' symbol
            ABSENTRY Entry           ; for absolute assembly: mark this as application entry point

; include derivative specific macros
            INCLUDE 'mc9s12c128.inc'

ROMStart    EQU  $4000               ; absolute address to place my code/constant data

; variable/data sectio
 ifdef _HCS12_SERIALMON
            ORG $3FFF - (RAMEnd - RAMStart)
 else
            ORG RAMStart
 endif

; Insert here your data definition.
Value     DC.B 1
Buttons   DC.B 1


; code section
            ORG   ROMStart
Entry:
            ; remap the RAM &amp; EEPROM here. See EB386.pdf
 ifdef _HCS12_SERIALMON
            ; set registers at $0000
            CLR   $11                ; INITRG= $0
            ; set ram to end at $3FFF
            LDAB  #$39
            STAB  $10                ; INITRM= $39

            ; set eeprom to end at $0FFF
            LDAA  #$9
            STAA  $12                ; INITEE= $9


            LDS   #$3FFF+1           ; See EB386.pdf, initialize the stack pointer
 else
            LDS   #RAMEnd+1          ; initialize the stack pointer
 endif
            CLI                      ; enable interrupts


            LDD   #0
            STD   PORTB              
            CLR   DDRA               ;configure port A for input
            LDAA  #$F0
            STAA  DDRB               ;configure port B as output


EndlessLoop:
            LDD   #0                 ; clear reg D
            LDAA  PORTA              ; load PORTA to reg A
            LDAB  PORTA              ; load PORTB to reg B
            ANDA  #$F0               ; get the 4 highest bits
            LSRA                     ; push 4 highest bits to lowest bits
            LSRA
            LSRA
            LSRA
            ANDB  #$08               ; get the 3rd bit
            EORB  #$08               ; switch values 
            STAA  Buttons            ; store reg A value to stack
            STAB  Value              ; store reg B value to stack
            LDAB  #$01               ; make reg B act as a counter
            LDAA  #$00               ; make reg A act as a total
Loop:
            CMPB  Buttons           ; check if reg B has reached N
            BHI   ButtonPressed     ; if it has, branch to ButtonPressed
            ABA                     ; add B to sum
            ADDB  #$01              ; Increment B by 1
            BRA   Loop              ; restart this Loop
ButtonPressed:
            LDAB  Value             ; load back the value
            CMPB  #$08              ; see if button 4 was pressed
            BNE   ButtonNotPressed  ; if not, branch to ButtonNotPressed
            ANDA  #$F0              ; get the 4 highest bits
            STAA  PORTB             ; show them on the LED
            BRA   EndlessLoop       ; restart main loop
ButtonNotPressed:
            ANDA  #$0F              ; get the 4 lowest bits to LED 4-7
            LSLA                    ; shift 4 times
            LSLA
            LSLA
            LSLA           
            STAA  PORTB             ; show the 4 lowest bits to LED 4-7
            BRA   EndlessLoop       ; restart loop

;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry             ; Reset Vector
