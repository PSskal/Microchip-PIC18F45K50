/* 
 * File:   maicode.c
 * Author: HOME
 *
 * Created on 22 de mayo de 2023, 09:57 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#include "EUSART.h"
#define _XTAL_FREQ 4000000UL


/*
 * 
 */
void CONFIG_Init(void){
    OSCCON = 0x52;
    
}

int main(int argc, char** argv) {
    CONFIG_Init();
    EUSART_Init();
    while(1){
        EUSART_Tx(0x33);
        __delay_ms(500);
        EUSART_Tx(0x99);
        __delay_ms(500);
    }
    
    return (EXIT_SUCCESS);
}

