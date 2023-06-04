/*
 sensor de temperatura lm35 utilizando un voltaje de referencia de 1 voltio en RA3 
 */


#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char nivel = 0;
unsigned char diezmillar,millar,centena,decena,unidad, decimales_unidad, decimales_decena;
unsigned char dino3[] ={0x1F,0x17,0x1F,0x1F,0x1C,0x1F,0x1C,0x1C};

//sensot mq-135
float RS_air, R0, Ratio;

void configuro(void){
    //configuracion del reloj principal
    OSCCON = 0x52;
    //configuracion de las E/S
    TRISAbits.RA0 = 1;
    ANSELAbits.ANSA0 = 1;
    TRISBbits.RB0 = 1;
    ANSELBbits.ANSB0 = 0;
    INTCON2bits.RBPU = 0;
    WPUBbits.WPUB0 = 1;
    TRISC = 0xBB;
    ANSELC = 0x3B;
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
    //LATCbits.LATC2 = 0;
    //configuracion del CCP1 para PWM
    PR2 = 62;
    CCPR1L = 31;                //duty cycle inicial 50%
    T2CON = 0x06;               //TMR2 on, 1:16 psc
    CCP1CON = 0x0C;             //CCP1 en modo PWM
    //confuguracion de interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0;
}

int ADC_Read(unsigned char channel) {
    ADCON0bits.CHS = channel;       // SELECCIONAR el canal de ENTRADA ANALAOGICA
    ADCON0bits.GO = 1;              // INICIAR LA CONVERSION
    
    while (ADCON0bits.GO);          // ESPERAR QUE LA CONVERSION COMPLETE
    
    return ((ADRESH << 8) + ADRESL); // COMBINAR LOS REGISTROS ADRESH y ADRESL para OBTENER EL RESULTADO COMPLETO
}

void convierte(float numero) {
    int entero = (int)numero;  // Parte entera del número
    int decimales = (int)(numero * 100) % 100;  // Parte decimal del número
    
//    diezmillar = numero / 10000;
//    millar = (numero % 10000) / 1000;
    centena = entero / 100;
    decena = (entero % 100) / 10;
    unidad = entero % 10;
    
    // Muestra los dígitos de la parte decimal (dos decimales)
    decimales_unidad = decimales % 10;
    decimales_decena = decimales / 10;
}

void main(void) {
    configuro();
    unsigned int adcValue;
    float mq135_volt;
    
    POS_CURSOR(1,0);
    DISPLAY_SCROLL_MESSAGE("  midiendo el voltaje y r0 de un senor MW-135");
    BORRAR_LCD(); 
    GENERATE_AND_DISPLAY_CUSTOM_CHAR(dino3,0, 2, 1);
    __delay_ms(1000);
    BORRAR_LCD(); 
    while(1){
        adcValue = ADC_Read(0);
        mq135_volt = (adcValue*5.0/1023);
        RS_air = (5.0 - mq135_volt)/mq135_volt;
        R0 = RS_air/9.8;
        //Ratio = RS_air/R0 para el gas
       
        ESCRIBE_MENSAJE2("Volt:");
        DISPLAY_FLOAT_VALUE(RS_air, 2);
        ENVIA_CHAR(0xDF);
        ENVIA_CHAR('C');
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("r0:");
        DISPLAY_FLOAT_VALUE(R0, 2);
        ENVIA_CHAR('  ');
        DISPLAY_INT_VALUE(2300);
          
    }
}

void __interrupt() INT0_ISR(void){
    INTCONbits.INT0IF = 0;
    switch(nivel){
        case 0:
            CCPR1L = 5;
            nivel = 1;
            break;
        case 1:
            CCPR1L = 15;
            nivel = 2;
            break;
        case 2:
            CCPR1L = 25;
            nivel = 3;
            break;
        case 3:
            CCPR1L = 35;
            nivel = 4;
            break;
        case 4:
            CCPR1L = 45;
            nivel = 5;
            break;
        case 5:
            CCPR1L = 55;
            nivel = 6;
            break;
        case 6:
            CCPR1L = 62;
            nivel = 0;
            break;
    }
}