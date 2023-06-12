/* 
 * File:   maicode.c
 * Author: HOME
 *
 * Created on 22 de mayo de 2023, 09:57 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k50.h>
#include "cabecera.h"
#include "EUSART.h"
#define _XTAL_FREQ 16000000UL


/*
 * 
 */
void CONFIG_Init(void){
    OSCCON = 0x72;
    TRISDbits.RD7 = 0;
    ANSELDbits.ANSD7 = 0;
    
}

int main(int argc, char** argv) {
    CONFIG_Init();
    EUSART_Init();
    while(1){
        EUSART_Cadena("por estar avansando en usart, se me olvidé enviar el foro");
        __delay_ms(1000);
        EUSART_Cadena("me olvide que habia puesto en guardar");
        __delay_ms(1000);
        EUSART_Cadena("kaaluuuun");
        __delay_ms(1000);
        
       
    }
    
    return (EXIT_SUCCESS);
}

