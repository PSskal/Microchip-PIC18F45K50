#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char e_acento[]={0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E,0x00};
unsigned char cuenta = 0;
unsigned char centena = 0;
unsigned char decena = 0;
unsigned char unidad = 0;
unsigned char horas = 14, minutos = 16, segundos = 0, ticks = 0;

void configuro(void){
    OSCCON = 0x52;              //INTOSC a 4MHz
    TRISD = 0x00;               //RD como salida
    ANSELD = 0x00;              //RD como digital
    TRISBbits.RB0 = 1;          //RB0 como entrada
    ANSELBbits.ANSB0 = 0;       //RB0 como digital
    INTCON2bits.RBPU = 0;       //weak pullup en RB activados
    TRISCbits.RC2 = 0;          //RC2 como salida
    ANSELCbits.ANSC2 = 0;       //RC2 como digital
    TRISAbits.RA0 = 1;          //RA0 como entrada
    ANSELAbits.ANSA0 = 1;       //RA0 como analogica
    TRISEbits.RE0 = 0;
    ANSELEbits.ANSE0 = 0;
    
    T1CON = 0x01;               //TMR1 On, fosc/4 psc 1:1
    CCP1CON = 0x0B;             //Modo comparador evento especial de disparo
    CCPR1H = 0x27;
    CCPR1L = 0x10;              //valor de referencia de comparacion 10000
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.CCP1IE = 1;
    
}

void LCD_init(void){
    __delay_ms(18);
    LCD_CONFIG();
    BORRAR_LCD();
    CURSOR_HOME();
    CURSOR_ONOFF(OFF);
    LATCbits.LC2 = 1;
    GENERACARACTER(e_acento,0);
}

void convierte(char numero){
    centena = numero / 100;
    decena = (numero % 100) / 10;
    unidad = numero % 10;
}

void main(void) {
    configuro();
    LCD_init();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE("Con fe aprobar");
    ENVIA_CHAR(0);
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE("GENTE UPC");
    __delay_ms(2000);
    BORRAR_LCD();
    while(1){
        POS_CURSOR(1,3);
        ESCRIBE_MENSAJE("CLOCK *_*");
        POS_CURSOR(2,3);
        convierte(horas);
        ENVIA_CHAR(decena+0x30);
        ENVIA_CHAR(unidad+0x30);
        ENVIA_CHAR(':');
        convierte(minutos);
        ENVIA_CHAR(decena+0x30);
        ENVIA_CHAR(unidad+0x30);
        ENVIA_CHAR(':');
        convierte(segundos);
        ENVIA_CHAR(decena+0x30);
        ENVIA_CHAR(unidad+0x30);
        ENVIA_CHAR(':');
        convierte(ticks);
        ENVIA_CHAR(decena+0x30);
        ENVIA_CHAR(unidad+0x30);
        
        if(PORTBbits.RB0==0){
            horas++;
        }
    }
}

void __interrupt() CCP1_ISR(void){
    PIR1bits.CCP1IF = 0;
    if(ticks == 99){
        ticks = 0;
        if(segundos == 59){
            segundos = 0;
            if(minutos == 59){
                minutos = 0;
                if(horas == 23){
                    horas = 0;
                }
                else{
                    horas++;
                }
            }
            else{
                minutos++;
            }
        }
        else{
            segundos++;
        }
    }
    else{
        ticks++;
    }
}