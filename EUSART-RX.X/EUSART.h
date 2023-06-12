#define _XTAL_FREQ 4000000UL
void EUSART_Init(void);
void EUSART_Tx(char data);
char EUSART_Rx(void);