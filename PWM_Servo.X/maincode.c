/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on 22 de mayo de 2023, 07:56 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#define _XTAL_FREQ 500000UL

/*
 * 
 */

void PWM_Init(void)
{
    // Configuración del TMR2 para PWM
    T2CONbits.T2CKPS = 0b11;  // Prescaler 1:16
    
    
    // Configuración del CCP1 para PWM
    CCP1CONbits.CCP1M = 0b1100;  // Modo PWM
    CCP1CONbits.DC1B0 = 0b00;          // Pin CCP1 configurado como salida PWM
    
    // Configuración del periodo del PWM
    PR2 = 155;  // Periodo del PWM (8 bits)
    
    /* Configuración del ciclo de trabajo inicial
     CCPRxL= PWD/(TOSC*PREESCALER) => 1000
    1000 = 100%
    CCPR1L = 80%          0B11001000 00 CC1 */
    
    CCPR1L = 31>>2;  // Ciclo de trabajo (5%)  
    
    
    T2CONbits.TMR2ON = 1;     // Habilitar 
    TMR2 = 0;
}

void main(void)
{   
    OSCCON = 0x22;
    // Configuración de pines
    TRISCbits.RC2 = 0;  // Pin RC2 como salida para el LED
    TRISDbits.RD1 = 1;
    TRISDbits.RD0 = 1;
    
    
    // Inicialización del PWM
    int maximo = 68>>2;
    PWM_Init();
    int duty_cicle;
    // Ciclo infinito
    while(1){
         for(duty_cicle= 1; duty_cicle<maximo; duty_cicle++){
            CCPR1L = duty_cicle;
            __delay_ms(100);
        }
        __delay_ms(2000);   
       
    }
    
}

