#ifndef USART_H
#define USART_H

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void USART_Init();
void USART_Transmit(char c);
char USART_Receive(); 
int USART_TimedReceive();

#endif