
#include <xc.h>
#include <string.h>
#include "cabecera.h"
#include "LCD.h"
#include "EUSART.h"
#define _XTAL_FREQ 8000000UL

unsigned char valor = 0;
unsigned char option = 3;
unsigned int pwmValue;
float adcValue; 
unsigned char dino3[] ={0x1F,0x17,0x1F,0x1F,0x1C,0x1F,0x1C,0x1C};
void configuro(void) {
    OSCCON = 0x62;

    // Configuración de E/S
    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;
    
    TRISAbits.RA3 = 1;
    ANSELAbits.ANSA3 = 1;
    TRISAbits.RA5 = 1;
    ANSELAbits.ANSA5 = 1;

    // Configuración del Timer0
    T0CONbits.TMR0ON = 1;   // Habilitar el Timer0
    T0CONbits.T08BIT = 0;   // Modo de 16 bits
    T0CONbits.T0CS = 0;     // Fuente de reloj interna (FOSC/4)
    T0CONbits.T0SE = 0;     // Borde de subida
    T0CONbits.PSA = 0;      // Utilizar el preescalador
    T0CONbits.T0PS = 0b000; // Preescalador de 1:256

    // Habilitar la interrupción del Timer0
    INTCONbits.TMR0IE = 1;  // Habilitar la interrupción del Timer0
    INTCONbits.GIE = 1;     // Habilitar las interrupciones globales
    INTCONbits.PEIE = 1;
    
    // Habilitar  la interrupción de recepciion de dato de EUSART
    PIE1bits.RC1IE = 1;         //interrupcion de recepcion encendida
    
    TRISD = 0x0F;               //rd4-rd7 salidas
    ANSELD = 0x0F;              //rd4-rd7 digitales
    TRISE = 0xF8;               //re0-re2 salidas
    ANSELE = 0xF8;              //re0-re2 digitales
    
    
    // Configuración del módulo ADC    
    ADCON0bits.ADON = 1;     // Habilitar el módulo ADC
    ADCON1bits.PVCFG0 = 0;  // VDD de referencia
    ADCON1bits.NVCFG0 = 0;  // VSS de referencia 
    ADCON2bits.ADFM = 1;    // Justificación a la derecha
    ADCON2bits.ADCS = 0b100; // FOSC/4 como velocidad de conversión
    ADCON2bits.ACQT = 0b100; // Tiempo de adquisición de 8 TAD 
  
    //Configuracion USART
    EUSART_Init();
    // Configuración del LCD
    LCD_INIT();
}

int ADC_Read(unsigned char channel) {
    ADCON0bits.CHS = channel;       // Seleccionar el canal de entrada analógica
    ADCON0bits.GO = 1;              // Iniciar la conversión
    
    while (ADCON0bits.GO);          // Esperar que la conversión se complete
    
    return ((ADRESH << 8) + ADRESL); // Combinar los registros ADRESH y ADRESL para obtener el resultado completo
}


void main(void) {
    configuro();
    unsigned int adcRead;
    
    POS_CURSOR(1,0);
    DISPLAY_SCROLL_MESSAGE("  no quiero desaprobar profesor Kaluuun");
    BORRAR_LCD(); 
    GENERATE_AND_DISPLAY_CUSTOM_CHAR(dino3,0, 2, 1);
    __delay_ms(1000);
    BORRAR_LCD(); 
    
    while (1) {
        // Tu código principal aquí
        
        if(option == 1){    //180 grados normal
            adcRead = ADC_Read(3);
            adcValue = (adcRead*2000.00)/1023+480;
            pwmValue = (int)adcValue; // Escalar el valor del potenciómetro para obtener el rango de 1,000 a 2,000
            float angle = ((pwmValue - 480) * 180.00) / (2480 - 480);
            POS_CURSOR(1, 0);
            ESCRIBE_MENSAJE2("Ang:");
            DISPLAY_FLOAT_VALUE(angle, 2);
            POS_CURSOR(2, 0);
            ESCRIBE_MENSAJE2("Raw:");
            DISPLAY_INT_VALUE(adcRead);
        }
        else if(option == 2){   //180 grados invertido
            adcRead = ADC_Read(3);
            adcValue = 2480 - ((adcRead * 2000.00) / 1023);
            pwmValue = (int)adcValue; // Escalar el valor del potenciómetro para obtener el rango invertido de 2480 a 480
            float angle = 180 - ((pwmValue - 480) * 180.00) / (2480 - 480);
            POS_CURSOR(1, 0);
            ESCRIBE_MENSAJE2("Ang:");
            DISPLAY_FLOAT_VALUE(angle, 2);
            POS_CURSOR(2, 0);
            ESCRIBE_MENSAJE2("Raw:");
            DISPLAY_INT_VALUE(adcRead);
        }else if(option == 3){  //90 grados normal
            adcRead = ADC_Read(3);
            adcValue = (adcRead * 905.00) / 1023 + 480;
            pwmValue = (int)adcValue; // Escalar el valor del potenciómetro para obtener el rango de 1,000 a 2,000
            float angle = ((pwmValue - 480) * 90.00) / (1385 - 480);
            POS_CURSOR(1, 0);
            ESCRIBE_MENSAJE2("Ang:");
            DISPLAY_FLOAT_VALUE(angle, 2);
            POS_CURSOR(2, 0);
            ESCRIBE_MENSAJE2("Raw:");
            DISPLAY_INT_VALUE(adcRead);
        }
    }
}

void __interrupt() isr() {
    if (INTCONbits.TMR0IF == 1) {
        if (valor == 0) {
            LATAbits.LA0 = 0;
            TMR0H = (65536 - (20000 - pwmValue)) >> 8;   // 18 ms
            TMR0L = (65536 - (20000 - pwmValue)) & 0xFF; 
            
            INTCONbits.TMR0IF = 0; // Limpiar la bandera de interrupción del Timer0
            valor = 1;
        }
        else if (valor == 1) {
            LATAbits.LA0 = 1;
            TMR0H = (65536 - pwmValue) >> 8;   // 2 ms
            TMR0L = (65536 - pwmValue) & 0xFF; 
            
            INTCONbits.TMR0IF = 0; // Limpiar la bandera de interrupción del Timer0
            valor = 0;
        }
    }
    else if(PIR1bits.RC1IF == 1){
        switch(RCREG1){
            case 'N':
                
                option = 1;
                EUSART_Cadena("Desplazamiento Normal");
                break;
            case 'Y':
               
                option = 2;
                EUSART_Cadena("Desplazamiento Invertido");
                break;
            case 'H':
                
                option = 3;
                EUSART_Cadena("Mitad de desplamiento");
                break;
            default:
                EUSART_Cadena("opcion errada intente de nuevo :)"); 
                break;
        }
    }
 }