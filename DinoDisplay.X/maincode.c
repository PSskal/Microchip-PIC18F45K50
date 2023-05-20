#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char dino1[]= {0x00,0x00,0x00,0x00,0x10,0x10,0x18,0x1C};
unsigned char dino2[] ={0x00,0x01,0x01,0x01,0x01,0x03,0x0F,0x1F};
unsigned char dino3[] ={0x1F,0x17,0x1F,0x1F,0x1C,0x1F,0x1C,0x1C};
unsigned char dino4[] ={0x10,0x18,0x18,0x18,0x00,0x10,0x00,0x00};
unsigned char dino5[] ={0x1F,0x1F,0x0F,0x07,0x03,0x03,0x02,0x03};
unsigned char dino6[] ={0x1F,0x1F,0x1F,0x1F,0x16,0x02,0x02,0x03};
unsigned char dino7[] ={0x1F,0x19,0x10,0x00,0x00,0x00,0x00,0x00};

void configuro(void){
    OSCCON = 0x52;
    //configuracion de E/S
    TRISD = 0x00;  //RD SALIDAS
    ANSELD = 0x00; //RD DIGITALES
    TRISAbits.TRISA0 = 1; //RA0 ENTRADA 
    ANSELAbits.ANSA0 = 1; //RA0 ANALOGICA
    TRISEbits.TRISE0 = 0; //RE0 SALIDA
    ANSELEbits.ANSE0 = 0; //RE0 DIGITAL
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB0 = 0;
    INTCON2bits.RBPU = 0;
    TRISCbits.TRISC2 = 0;
    ANSELCbits.ANSC2 = 0;
    LATCbits.LATC2 = 1;  //encendemos el blacklight del LCD
    
    
    
  
    
}
void LCD_Init(void){
    __delay_ms(29);
    LCD_CONFIG();
    BORRAR_LCD();
    CURSOR_HOME();
    CURSOR_ONOFF(OFF);
    GENERACARACTER(dino1,0);
    GENERACARACTER(dino2,1);
    GENERACARACTER(dino3,2);
    GENERACARACTER(dino4,3);
    GENERACARACTER(dino5,4);
    GENERACARACTER(dino6,5);
    GENERACARACTER(dino7,6);
    
    
}
void main(void) {
    configuro();
    LCD_Init();
    while(1){
        
        for(int a=0; a<=18; a++){
            if(a>=3){
                 POS_CURSOR(1,a-3);
                 ENVIA_CHAR(0);
            }
            if(a>=2){
                POS_CURSOR(1,a-2);
                 ENVIA_CHAR(1);
            }
            if(a>=1){
               POS_CURSOR(1,a-1);
                ENVIA_CHAR(2);
            }
            POS_CURSOR(1,a);
            ENVIA_CHAR(3);
            if(a>=3){
                POS_CURSOR(2,a-3);
                ENVIA_CHAR(4);
            }
            if(a>=2){
                POS_CURSOR(2,a-2);
                 ENVIA_CHAR(5);
            }
            if(a>=1){
               POS_CURSOR(2,a-1);
                ENVIA_CHAR(6);
            }
            POS_CURSOR(1,a+4);
            ESCRIBE_MENSAJE("NO ME DEJES");
            POS_CURSOR(2,a+5);
            ESCRIBE_MENSAJE("ESPERAME!");
            __delay_ms(400);
            BORRAR_LCD();
        }        
    }        
}