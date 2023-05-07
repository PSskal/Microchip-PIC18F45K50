
PROCESSOR 18F45K50
#include "cabecera.inc"
    PSECT upcino, class=CODE, reloc=2, abs
    
upcino:
    ORG 000000H
    bra configuro 
    
    ORG 000008H	    ;la ubicacion del vector de high priority
    bra INT_2    ;interrupcion de high priority
    
    ORG 000018H 
    bra INT_1
    
    
    ORG 000020H
    
configuro:
    movlb 0FH
    movlw 52H
    movwf OSCCON
    
    bcf TRISD, 0; PUERTO D -> SALIDA TRISD = 0;
    bcf ANSELD, 0
    
    bsf TRISB, 0;  TRISB = W -> PUERTO B ENTRADA
    bcf ANSELB, 0

    BSF	INTCON3,7; INT2 -> ALTA PRIORIDAD -> 1US
    BCF	INTCON3,6; INT1 -> BAJA PRIORIDAD
    BSF	INTCON3,3; INT1 -> HABILITADA
    BSF	INTCON3,4; INT2 -> HABILITADA
    BCF	INTCON3,1
    BCF	INTCON3,0; FLAG LIMPIAMOS
    BSF	RCON,7; HABILITAMOS PRIORIDADES
    BCF	INTCON2,5
    BCF	INTCON2,4; 1 -> 0
    BSF	INTCON,7; GIEH -> ALTA PRIORIDAD
    BSF	INTCON,6; GIEL -> BAJA PRIORIDAD
inicio:
   nop
   bra	inicio

INT_2:
  BTFSS	PORTD,1; SI PORTD 1 == 1
  GOTO	ENCENDER_2
  BCF	LATD,1; LATD1 = 0
  GOTO	FIN_INT_2
ENCENDER_2:
  BSF	LATD,1
FIN_INT_2:
  BCF	INTCON3,1; INT2IF = 0; 
  RETFIE 
  
INT_1:
  BTFSS	PORTD,2; PORTD2 == 0
  GOTO	ENCENDER_1
  BCF	LATD,2; LATD2 = 0
  GOTO	FIN_INT_1
ENCENDER_1:
  BSF	LATD,2
FIN_INT_1:
  BCF	INTCON3,0; INT1IF = 0; 
  RETFIE 

  end upcino
