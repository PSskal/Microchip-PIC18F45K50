/* 
 * File:   maicode.c
 * Author: HOME
 *
 * Created on 22 de mayo de 2023, 01:57 PM
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
    TRISD = 0;
    ANSELD = 0;
    
}

void main(void) {
    CONFIG_Init();
    EUSART_Init(); 
    while(1){
      
       LATD = EUSART_Rx();
       
    }
    
    return;
}

