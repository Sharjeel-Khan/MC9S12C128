;*****************************************************************
;* 15-348 Spring 2016 Assignment Task 6 By Sharjeel Khan         *
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
Switches   DC.B   1
Buttons    DC.B   1

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
            LDAA  #$01
            STAA  DDRB               ;configure port B as output


EndlessLoop:
            LDD   #0                 ; clear reg D
            LDAB  PORTA              ; load PORTA to reg B
            ANDB  #$08               ; get the 3rd bit
            EORB  #$08               ; switch values 
            STAA  Buttons            ; store reg A value to stack
            STAB  Switches           ; store reg B value to stack
ButtonPressed:
            LDAB  Switches           ; load back the value
            CMPB  #$08               ; see if button 4 was pressed
            BNE   EndlessLoop        ; if not, restart main loop
Continue:
            LDAA  PORTA
            ANDA  #$F0               ; get the 4 highest bits
            LSRA                     ; push 4 highest bits to lowest bits
            LSRA
            LSRA
            LSRA
            CMPA  #$01               ; Jump to do clockwise low speed
            BEQ   NOne
            CMPA  #$02               ; Jump to do clockwise medium speed
            BEQ   NTwo
            CMPA  #$03               ; Jump to do clockwise high speed
            BEQ   NThree
            CMPA  #$04               ; Jump to do anticlockwise low speed
            BEQ   NFour
            CMPA  #$05               ; Jump to do anticlockwise medium speed
            BEQ   NFive
            CMPA  #$06               ; Jump to do anticlockwise high speed
            BEQ   NSix
            BRA   Ending   
NOne:
            LDAA  #$01
            STAA  PORTB
            LDX   #$00
One:
            CPX   #425               ; Begin looping to make motor move clockwise low speed
            BEQ   Delay
            INX
            BRA   One 
NTwo:
            LDAA  #$01
            STAA  PORTB
            LDX   #$00
Two:
            CPX   #420               ; Begin looping to make motor move clockwise medium speed
            BEQ   Delay
            INX
            BRA   Two
NThree:
            LDAA  #$01
            STAA  PORTB
            LDX   #$00
Three:
            CPX   #400                ; Begin looping to make motor move clockwise high speed
            BEQ   Delay
            INX
            BRA   Three
NFour:
            LDAA  #$01
            STAA  PORTB
            LDX   #$00
Four:
            CPX   #440                 ; Begin looping to make motor move anticlockwise low speed
            BEQ   Delay
            INX
            BRA   Four
NFive:
            LDAA  #$01
            STAA  PORTB
            LDX   #$00
Five:
            CPX   #450                 ; Begin looping to make motor move anticlockwise medium speed
            BEQ   Delay
            INX
            BRA   Five
NSix:
            LDAA  #$01
            STAA  PORTB
            LDX   #$00
Six:
            CPX   #460                  ; Begin looping to make motor move anticlockwise high speed
            BEQ   Delay
            INX
            BRA   Six
Delay:
            LDAA  #$00
            STAA  PORTB
            LDX   #$00
Loop:
            CPX   #$19A0
            BEQ   Ending
            INX
            BRA   Loop
Ending:            
            LBRA   Continue        ; restart main loop

;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry             ; Reset Vector
