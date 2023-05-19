/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on 19 de mayo de 2023, 07:47 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL
/*
 * 
 */
void conguration(void){
    OSCCON = 0X52;      //INTOSC A 4MHz
    
    TRISD = 0X00;       //RD salidas
    ANSELD = 0X00;      //RD digitales
    TRISCbits.TRISC2 = 0; //RC2 salida
    ANSELCbits.ANSC2 = 0; //RC2 digital
}

void LCD_init(void){
    __delay_ms(28);
    LCD_CONFIG();
    BORRAR_LCD();
    CURSOR_HOME();
    CURSOR_ONOFF(OFF);
    LATCbits.LATC2 = 1;
}

int main(int argc, char** argv) {
    conguration();
    LCD_init();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE("HOLA MUNDO!");
    __delay_ms(200);
    
    return (EXIT_SUCCESS);
}

