/*
 sensor de temperatura lm35 utilizando un voltaje de referencia de 1 voltio en RA3 
 */


#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned int adcValue;
void configuro(void){
    //configuracion del reloj principal
    OSCCON = 0x52;
    //configuracion de las E/S
    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;
    
    TRISAbits.RA1 = 1;
    ANSELAbits.ANSA1 = 1;
    
    TRISD = 0x08;
    ANSELD = 0x00;
    
    
    //CONFIGURACION DEL MODULO ADC    
    ADCON0bits.ADON = 1;     // HABILITAR EL MODULO ADC
    ADCON1bits.PVCFG0 = 0;       //VDD DE REFERENCIA
    ADCON1bits.NVCFG0 = 0;       //VSS DE REFERENCIA 
    ADCON2bits.ADFM = 1;     // JUSTIFICACION A LA DERECHA
    ADCON2bits.ADCS = 4;     // FOSC/4 como VELOCIDAD DE CONVERSION
    ADCON2bits.ACQT = 4;     // TIEMPO DE ADQUISICION de 8 TAD 
    //VREFCON0 = 0xD0;        //VRef habilitado con salida 1.024V para FVR_buf2
    
    //configuracion del LCD
    LCD_INIT();
    
    
    //configuracon del timer0
    T0CONbits.TMR0ON = 1;     // Habilitar el Timer 0
    T0CONbits.T08BIT = 1;     // Modo de 8 bits
    T0CONbits.T0CS = 0;       // Fuente de reloj interna (FOSC/4)
    T0CONbits.T0SE = 0;       // Borde de subida
    T0CONbits.PSA = 0;        // Utilizar el preescalador
    T0CONbits.T0PS = 0b110;   // Preescalador de 1:8
   
    
    INTCONbits.PEIE = 1;     // Habilitar las interrupciones de periféricos
    INTCONbits.TMR0IE = 1;    // Habilitar la interrupción del Timer 0
    INTCONbits.GIE = 1;       // Habilitar las interrupciones globales
                
    
}

int ADC_Read(unsigned char channel) {
    ADCON0bits.CHS = channel;       // SELECCIONAR el canal de ENTRADA ANALAOGICA
    ADCON0bits.GO = 1;              // INICIAR LA CONVERSION
    
    while (ADCON0bits.GO);          // ESPERAR QUE LA CONVERSION COMPLETE
    
    return ((ADRESH << 8) + ADRESL); // COMBINAR LOS REGISTROS ADRESH y ADRESL para OBTENER EL RESULTADO COMPLETO
}


void main(void) {
    configuro();
    TMR0 = 100;                   // Valor inicial del Timer 0 para un intervalo de 10 ms
    
    while(1) {
        adcValue = ADC_Read(1);
        unsigned int pwmValue = (adcValue * 100) / 1023 + 100; // Escalar el valor del potenciómetro para obtener el rango de 125 a 250
        
        // Control del servo
        if (TMR0L >= pwmValue) {
            LATAbits.LATA0 = 0;       // Encender el servo
        }
        
        if (TMR0L >= 250) {
            LATAbits.LATA0 = 1;       // Apagar el servo después de 20 ms
           
        }
    }
}

void __interrupt() isr() {
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {  // Verificar si la interrupción del Timer 0 ocurrió y está habilitada
        INTCONbits.TMR0IF = 0;                      // Limpiar la bandera de interrupción del Timer 0
        TMR0L = 100;                                 // Reiniciar el Timer 0 para el próximo ciclo de trabajo
    }
}