/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on 21 de mayo de 2023, 09:13 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

/*
 * 
 */
unsigned char numeros[]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};

                        // a   b   c     d   e   f      g   h    i    j    k    l    m   n      o   p    q   r      s    t    u   v     w    x    y    z      
unsigned char letras[]= {0x77,0x7,0x39,0x5E,0x79,0x71,0x6F,0x74,0x30,0x1E,0x76,0x38,0x15,0x54,0x3F,0x73,0x67,0x60,0x6D,0x78,0x3E,0x1C,0x2A,0x76,0x6E,0x5B};
void configuration(){
    OSCCON = 0x52;
    TRISD = 0;
    ANSELD = 0;
    
}
int main(int argc, char** argv) {
    
    configuration();
    char i = 0;
    while(1){
        LATD = numeros[i];
        __delay_ms(1000);
        if(i==10){
            i=0;
        }else{
        i++;
        }
    }
    
    return (EXIT_SUCCESS);
}

