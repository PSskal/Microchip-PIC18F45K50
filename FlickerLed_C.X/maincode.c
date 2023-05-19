/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on 19 de mayo de 2023, 07:15 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

/*
 * 
 */
void configuration(void){
    OSCCON = 0X52;
    
    TRISEbits.TRISE0 = 0;
    ANSELEbits.ANSE0 = 0;
}

int main(int argc, char** argv) {
    configuration();
    __delay_ms(200);
    LATEbits.LATE0 = ~LATEbits.LATE0;
    
    return (EXIT_SUCCESS);
}

