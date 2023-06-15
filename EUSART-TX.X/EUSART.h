#define _XTAL_FREQ 16000000UL
void EUSART_Init(void);
void EUSART_Tx(char data);
char EUSART_Rx(void);
void EUSART_Cadena(char *str);