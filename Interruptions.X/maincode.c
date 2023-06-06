/*
 * File:   maincode.c
 * Author: Richard Pascual @PSskal
 *
 * Created on 5 de junio de 2023, 06:15 PM
 */


#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned int counter = 0;

void INIT_Config(){
    //configuracion del reloj principal
    OSCCON = 0x52;
    //configuracion de las E/S
    TRISBbits.RB0 = 1;
    ANSELBbits.ANSB0 = 0;
    TRISBbits.RB1 = 1;
    ANSELBbits.ANSB1 = 0;
    TRISBbits.RB2 = 1;
    ANSELBbits.ANSB2 = 0;
    INTCON2bits.RBPU = 0;
    WPUBbits.WPUB0 = 1;
    TRISC = 0xBB;
    ANSELC = 0x3B;
    TRISD = 0x08;
    ANSELD = 0x00;
    
    //configuracion del LCD
    LCD_INIT();
    
    //configuracion del CCP1 para PWM
    PR2 = 62;
    CCPR1L = 15;                //duty cycle inicial 50%
    T2CON = 0x06;               //TMR2 on, 1:16 psc
    CCP1CON = 0x0C;             //CCP1 en modo PWM
    
     //confuguracion de interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0;
    INTCON3bits.INT1IE = 1;   // Habilitar la interrupción externa INT1
    INTCON2bits.INTEDG1 = 0; // Detectar flanco de bajada en INT1
   
}
void main(void) {
  
    INIT_Config();
    while(1){
    POS_CURSOR(1,0);
    DISPLAY_SCROLL_MESSAGE("hola");
    POS_CURSOR(2,0);
    DISPLAY_INT_VALUE(counter);
    }
    
    return;
}

void __interrupt() ISR(void)
{
    if (INTCONbits.INT0IF){
        // Rutina de interrupción para INT0
        counter++;
        INTCONbits.INT0IF = 0; // Limpiar el flag de interrupción de INT0
    } else if (INTCON3bits.INT1IF)
    {
        // Rutina de interrupción para INT1
        counter--;  // Decrementar el contador en 1
        INTCON3bits.INT1IF = 0; // Limpiar el flag de interrupción de INT1
    }
}