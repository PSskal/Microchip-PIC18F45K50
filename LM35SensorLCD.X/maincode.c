/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on May 20, 2023, 7:59 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL
/*
 * 
 */

unsigned char centena = 0;
unsigned char decena = 0;
unsigned char unidad = 0;
unsigned char decimales_unidad, decimales_decena;

void configuration(void){
    
    OSCCON = 0x52;              //INTOSC a 4MHz
    TRISD = 0x00;               //RD como salida
    ANSELD = 0x00;              //RD como digital
    TRISBbits.RB0 = 1;          //RB0 como entrada
    ANSELBbits.ANSB0 = 0;       //RB0 como digital
    INTCON2bits.RBPU = 0;       //weak pullup en RB activados
    TRISCbits.RC2 = 0;          //RC2 como salida
    ANSELCbits.ANSC2 = 0;       //RC2 como digital
    
    TRISA = 0XFF;
    ANSELA = 0XFF;
    
    //CONFIGURACION DEL MODULO ADC    
    ADCON1bits.PVCFG0 = 0;       //VDD DE REFERENCIA
    ADCON1bits.NVCFG0 = 0;       //VSS DE REFERENCIA 
    ADCON2bits.ADFM = 1;     // JUSTIFICACION A LA DERECHA
    ADCON2bits.ADCS = 1;     // FOSC/8 como VELOCIDAD DE CONVERSION
    ADCON2bits.ACQT = 2;     // TIEMPO DE ADQUISICION de 4 TAD 
    ADCON0bits.ADON = 1;     // HABILITAR EL MODULO ADC
}

int ADC_Read(unsigned char channel) {
    ADCON0bits.CHS = channel;       // SELECCIONAR el canal de ENTRADA ANALAOGICA
    ADCON0bits.ADON = 1;            // HABILITAR EL MODULO ADC
    ADCON0bits.GO = 1;              // INICIAR LA CONVERSION
    
    while (ADCON0bits.GO);          // ESPERAR QUE LA CONVERSION COMPLETE
    
    return ((ADRESH << 8) + ADRESL); // COMBINAR LOS REGISTROS ADRESH y ADRESL para OBTENER EL RESULTADO COMPLETO
}

void LCD_init(void){
    __delay_ms(18);
    LCD_CONFIG();
    BORRAR_LCD();
    CURSOR_HOME();
    CURSOR_ONOFF(OFF);
    LATCbits.LC2 = 1;
}

void convierte(float numero) {
    int entero = (int)numero;  // Parte entera del número
    int decimales = (int)(numero * 100) % 100;  // Parte decimal del número
    
    centena = entero / 100;
    decena = (entero % 100) / 10;
    unidad = entero % 10;
    
    // Muestra los dígitos de la parte decimal (dos decimales)
    decimales_unidad = decimales % 10;
    decimales_decena = decimales / 10;
}


int main(int argc, char** argv) {
    configuration();
    LCD_init();
    unsigned int adcValue;
    float temperatura;
    char datos[20];

     while (1) {
        adcValue = ADC_Read(0);      // Leer el valor DEL canal 0 (AN0)
        temperatura = (500.0*adcValue)/1023;
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE("TEMP:");
        convierte(temperatura);
        ENVIA_CHAR(centena+0x30);
        ENVIA_CHAR(decena+0x30);
        ENVIA_CHAR(unidad+0x30);
        ESCRIBE_MENSAJE(".");
        ENVIA_CHAR(decimales_decena+0x30);
        ENVIA_CHAR(decimales_unidad+0x30);
        ESCRIBE_MENSAJE(" C");
        
        //SEGUNDA MANERA DE DECLAR DECIMALES 
        POS_CURSOR(1,0);
        sprintf(datos,"%.2f",temperatura);
        ESCRIBE_MENSAJE("TEMP:");
        ESCRIBE_MENSAJE(datos);
        ESCRIBE_MENSAJE(" C");
        
        __delay_ms(3000);    // RETARDO de 100 MS ENTRE CADA LECTURA DEL ADC
    }   
    return (EXIT_SUCCESS);
}

