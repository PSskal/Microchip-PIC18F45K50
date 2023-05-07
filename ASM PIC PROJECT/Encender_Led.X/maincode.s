PROCESSOR 18F45K50
#include "cabecera.inc"
    PSECT upcino, class=CODE, reloc=2, abs
    
upcino:
    ORG 000000H
    bra configuro 
    
    ORG 000020H
    
configuro:
    movlb 0FH
    movlw 52H
    movwf OSCCON
    
    bcf TRISD, 0	;declaramos el puerto como salida
    bcf ANSELD, 0	;declareamos el puerto como digital

    bsf TRISD, 1	;declaramos el puerto d1 como entrada
    bcf ANSELD, 1	;declaramos el puuerto como entrada digital
    
    clrf PORTD
inicio:
    
    btfss PORTD, 1
    bra inicio
    bsf LATD, 0		;le damos como 1 al puerto
    bra rebote
    
rebote:
    btfsc PORTD, 1
    bra rebote
    bra inicio
    end upcino