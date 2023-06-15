/* 
 * File:   maicode.c
 * Author: @PSskal
 *
 * Created on 12 de junio de 2023, 09:57 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k50.h>
#include "cabecera.h"
#include "EUSART.h"
#include "LCD.h"
#define _XTAL_FREQ 16000000UL


/*
 * 
 */
void CONFIG_Init(void){
    OSCCON = 0x72;
    TRISD = 0x08;
    ANSELD = 0x00;
    
}

int main(int argc, char** argv) {
    CONFIG_Init();
    LCD_INIT();
    EUSART_Init();
    
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("QUE FUE LOCO");
        EUSART_Cadena("esto es una transmicion por bluethoo");
        __delay_ms(1000);      
    }
    
    return (EXIT_SUCCESS);
}

