#include <xc.h>
#include "LCD.h"
#include <string.h>
#include <stdio.h>

void POS_CURSOR(unsigned char fila,unsigned char columna)
{
	if(fila == 1)
	{
		ENVIA_LCD_CMD(0x80+columna);
	}
	else if(fila == 2)
	{
		ENVIA_LCD_CMD(0xC0+columna);
	}
}

void BLINK_CURSOR(unsigned char val)
{
	if(val == OFF) ENVIA_LCD_CMD(0x0E);
	if(val == ON ) ENVIA_LCD_CMD(0x0F);
}

void DISPLAY_ONOFF(unsigned char estado)
{
	if(estado == ON)  ENVIA_LCD_CMD(0x0F);
	if(estado == OFF) ENVIA_LCD_CMD(0x08);
}

void CURSOR_HOME(void)
{
	ENVIA_LCD_CMD(0x02);
}

void CURSOR_SHIFTLEFT(void)
{
	ENVIA_LCD_CMD(0x10);
}

void CURSOR_SHIFTRIGHT(void)
{
	ENVIA_LCD_CMD(0x14);
}

void DISPLAY_SHIFTLEFT(void)
{
	ENVIA_LCD_CMD(0x18);
}

void DISPLAY_SHIFTRIGHT(void)
{
	ENVIA_LCD_CMD(0x1C);
}

void CURSOR_ONOFF(unsigned char estado)
{	
	if(estado == ON)	ENVIA_LCD_CMD(0x0E);
	if(estado == OFF)	ENVIA_LCD_CMD(0x0C);
}

void ESCRIBE_MENSAJE(const char *cadena,unsigned char tam)
{
	unsigned char i = 0;
	for(i = 0; i<tam; i++)
	{
		ENVIA_CHAR(cadena[i]);
	}
}

void ESCRIBE_MENSAJE2(const char *cadena)
{
    unsigned char tam;
    tam = strlen(cadena);
	unsigned char i = 0;
	for(i = 0; i<tam; i++)
	{
		ENVIA_CHAR(cadena[i]);
	}
}

void ENVIA_CHAR(unsigned char dato)
{
	unsigned char aux;
	RS = 1;
	LEER_LCD();
	TRISD = 0x00;
    __delay_us(100);
	RW = 0;
	E = 0;
	RS = 1;
	aux = dato & 0xF0;
	ENVIA_NIBBLE(aux);
	aux = dato << 4;
	ENVIA_NIBBLE(aux);	
}

void BORRAR_LCD(void)
{
	ENVIA_LCD_CMD(0x01);
}

void LCD_CONFIG(void)
{
	RS = 0;
	RW = 0;
	ENVIA_NIBBLE(0x30);
    __delay_ms(2);
	ENVIA_NIBBLE(0x30);
    __delay_us(100);
    ENVIA_NIBBLE(0x30);
	ENVIA_NIBBLE(0x20);
	ENVIA_LCD_CMD(0x01);
	ENVIA_LCD_CMD(0x28);
	ENVIA_LCD_CMD(0x0F);
	ENVIA_LCD_CMD(0x06);
	ENVIA_LCD_CMD(0x01);
}

void ENVIA_NIBBLE(unsigned char dato)
{
	LATD &= 0x0F;
	dato &= 0xF0;
	LATD|= dato;
	E = 1;
    __delay_us(100);
	E = 0;
}

void ENVIA_LCD_CMD(unsigned char dato)
{
	unsigned char aux;
	RS = 0;
	LEER_LCD();
	TRISD = 0b00000000;
    __delay_us(100);
    RW = 0;
	E = 0;
	RS = 0;
	aux = dato & 0xF0;
	ENVIA_NIBBLE(aux);
	aux = dato<<4;
	ENVIA_NIBBLE(aux);
}

void LEER_LCD(void)
{
	unsigned char aux;	
	TRISD = 0xF8;
	RS = 0;
	RW = 1;
	E = 1;
    __delay_us(100);
	aux = PORTD;
	E = 0;
    __delay_us(100);
	E = 1;
    __delay_us(100);
	E = 0;
	aux = aux & 0x80;
	while(aux == 0x80)
        {
            E = 1;
            __delay_us(100);
            aux = PORTD;
            E = 0;
            __delay_us(100);
            E = 1;
            __delay_us(100);
            E = 0;
            aux = aux & 0x80;
	}
}

// Función para mostrar un valor entero en el display
void DISPLAY_INT_VALUE(int value)
{
    char buffer[17];
    sprintf(buffer, "%d", value);
    ESCRIBE_MENSAJE(buffer, strlen(buffer));
}

// Función para mostrar un valor de punto flotante en el display
void DISPLAY_FLOAT_VALUE(float value, int decimalPlaces)
{
    char buffer[17];
    sprintf(buffer, "%.*f", decimalPlaces, value);
    ESCRIBE_MENSAJE(buffer, strlen(buffer));
}

// Función para desplazar automáticamente un mensaje largo

void DISPLAY_SCROLL_MESSAGE(const char* message)
{
    unsigned int len = strlen(message);
    unsigned int i;

    if (len <= 16)
    {
        ESCRIBE_MENSAJE(message, len);
    }
    else
    {
        for (i = 0; i <= len - 16; i++)
        {
            ESCRIBE_MENSAJE(message + i, 16);
            __delay_ms(500);
            BORRAR_LCD();
        }

        // Mostrar el final del mensaje si no se completa en un desplazamiento completo
        if (i < len)
        {
            ESCRIBE_MENSAJE(message + i, len - i);
            __delay_ms(1000);
        }
    }
}


// Función para generar y mostrar un carácter personalizado directamente en el display
void GENERATE_AND_DISPLAY_CUSTOM_CHAR(const unsigned char* pattern, unsigned char position, unsigned char Y, unsigned char X)
{
    unsigned char i;
    
    // Generar el carácter personalizado en la CGRAM
    ENVIA_LCD_CMD(0x40 + (8 * position));
    for (i = 0; i < 8; i++)
    {
        ENVIA_CHAR(pattern[i]);
    }
    POS_CURSOR(Y, X);  // Colocarse en la posición deseada
    // Mostrar el carácter personalizado en la DDRAM
    ENVIA_CHAR(position);  // Enviar el código del carácter personalizado
}



void LCD_INIT(void){
    __delay_ms(15);
    LCD_CONFIG();
    __delay_ms(15);
    BORRAR_LCD();
    CURSOR_HOME();
    CURSOR_ONOFF(OFF);
}
