#include <xc.h>
#include "EUSART.h"
#include "cabecera.h"
#define _XTAL_FREQ 8000000UL
#define BAUD_RATE 9600
#define SPBRG_VALUE ((_XTAL_FREQ / BAUD_RATE) / 64) - 1


void EUSART_Init(void){
   
    //configuracion del EUSART
    
    TRISCbits.RC6 = 0;          //rc6 salida
    ANSELCbits.ANSC6 = 0;       //rc6 digital
    TRISCbits.RC7 = 1;          //rc7 entrada
    ANSELCbits.ANSC7 = 0;       //rc7 digital
    
    
    TXSTAbits.BRGH = 1;         //BRGH = 1
    SPBRGH1 = 0;
    SPBRG1 = 51;                //vtx de 9600 baudios
    TXSTAbits.TXEN = 1;         //encendemos el transmisor del eusart
    RCSTAbits.SPEN = 1;         //encendemos el modulo eusart
    RCSTAbits.CREN = 1;         //encendemos el receptor
    PIE1bits.RC1IE = 1;         //interrupcion del receptor del eusart enabled   
}
void EUSART_Tx(char data){
    // Esperar hasta que el registro de transmisión esté vacío
    while (!TXSTAbits.TRMT);
    //Enviar datp
    TXREG1 = data;
}
char EUSART_Rx(void){
     // Esperar hasta que se reciba un dato completo
    while (!PIR1bits.RCIF);
    //recibit dato
    return RCREG1;
}

void EUSART_Cadena(char *str) {
    while (*str != '\0') {
        EUSART_Tx(*str);
        str++;
    }
    EUSART_Tx('\r');  // Caracter de retorno de carro ('\r')
    EUSART_Tx('\n');  // Caracter de nueva línea ('\n')
}

