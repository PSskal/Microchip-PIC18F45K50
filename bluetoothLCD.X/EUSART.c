#include <xc.h>
#include "EUSART.h"
#include "cabecera.h"
#define _XTAL_FREQ 16000000
#define BAUD_RATE 9600
#define SPBRG_VALUE ((_XTAL_FREQ / BAUD_RATE) / 64) - 1


void EUSART_Init(void){
   
      // Configurar los pines de E/S para el USART
    TRISCbits.RC6 = 0;   // Pin RC6 (TX) como salida
    TRISCbits.RC7 = 1;   // Pin RC7 (RX) como entrada

    // Configurar la velocidad de transmisi�n (baud rate)
    
    SPBRG1 = SPBRG_VALUE; //(unsigned char)((_XTAL_FREQ/BAUD))/64-1;        // Valor para baud rate de 9600 a Fosc = 4MHz

    // Configurar el m�dulo EUSART
    TXSTAbits.TXEN = 1;  // Habilitar la transmisi�n *
    RCSTAbits.SPEN = 1;  // Habilitar el USART
    RCSTAbits.CREN = 1;  // Habilitar la recepci�n continua

    // Configurar los bits de tama�o de datos y modo de operaci�n (8 bits, sin paridad)
    TXSTAbits.TX9 = 0;   // 8 bits de datos
    RCSTAbits.RX9 = 0;   // 8 bits de datos

    // Configurar el modo de operaci�n (as�ncrono, alta velocidad)
    BAUDCONbits.BRG16 = 0;  // Modo de 8 bits de baud rate
    TXSTAbits.SYNC = 0;     // Modo as�ncrono
    TXSTAbits.BRGH = 0;     // asincrono low speed
   
   
}
void EUSART_Tx(char data){
    // Esperar hasta que el registro de transmisi�n est� vac�o
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
    EUSART_Tx('\n');  // Caracter de nueva l�nea ('\n')
}

