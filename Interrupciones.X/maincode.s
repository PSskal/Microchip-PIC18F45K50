PROCESSOR 18F45K50
#include "cabecera.inc"
    PSECT upcino, class=CODE, reloc=2, abs
    
upcino:
    ORG 000000H
    bra configuro 
    
    ORG 000008H	    ;la ubicacion del vector de high priority
    bra INT0_ISR    ;interrupcion de high priority
    
    
    
    ORG 000020H
    
configuro:
    movlb 0FH
    movlw 52H
    movwf OSCCON
    
    bcf TRISD, 0; PUERTO D -> SALIDA TRISD = 0;
    bcf ANSELD, 0
    
    bsf TRISB, 0;  TRISB = W -> PUERTO B ENTRADA
    bcf ANSELB, 0
    BSF	INTCON,7; HABILITA INTERRUPCIONES GLOBALES
    BSF	INTCON,6; HABILITAMOS PERIFERICOS
    BSF	INTCON,4; HABILITAMOS INT0
    BCF	INTCON,1; LIMPIAMOS EL FLAG 
    ;BSF	INTCON2,6 
  ;BSF = BIT DEL REGISTRO -> 1
  ;BCF = BIT DEL REGISTRO -> 0 
inicio:
   nop
   bra	inicio

INT0_ISR:
  BTFSS	PORTD,0; SI PORTD 0 == 1
  GOTO	encender
  BCF	LATD,0; LATD0 = 0
  GOTO	fin_ext
encender:
  BSF	LATD,0
fin_ext:
  BCF	INTCON,1; INTOIF = 0; 
  RETFIE

  end upcino


