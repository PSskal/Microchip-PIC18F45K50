/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on 21 de mayo de 2023, 03:56 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

/*
 * 
 */

void PWM_Init(void)
{
    // Configuración del TMR2 para PWM
    T2CONbits.T2CKPS = 0b01;  // Prescaler 1:16
    
    
    // Configuración del CCP1 para PWM
    CCP1CONbits.CCP1M = 0b1100;  // Modo PWM
    CCP1CONbits.DC1B0 = 0b00;          // Pin CCP1 configurado como salida PWM
    
    // Configuración del periodo del PWM
    PR2 = 250;  // Periodo del PWM (8 bits)
    
    /* Configuración del ciclo de trabajo inicial
     CCPRxL= PWD/(TOSC*PREESCALER) => 1000
    1000 = 100%
    CCPR1L = 80%          0B11001000 00 CC1 */
    
    CCPR1L = 0b11001000;  // Ciclo de trabajo (80%)  
    TMR2 = 0;
    
    T2CONbits.TMR2ON = 1;     // Habilitar TMR2
}

void main(void)
{   
    OSCCON = 0x52;
    // Configuración de pines
    TRISCbits.RC2 = 0;  // Pin RC2 como salida para el LED
    
    // Inicialización del PWM
    PWM_Init();
    
    // Ciclo infinito
    while(1)
    {
        
    }
}

