#include <avr/io.h>
#include <util/delay.h>
#include "../inc/USART.h"


void USART_Init(){
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE & 0xFF;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);   // Habilita TX e RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits, 1 stop, sem paridade
}


// Envia um caractere
void USART_Transmit(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}


// Recebe um caractere (bloqueante)
char USART_Receive() {
    while ((!(UCSR0A & (1 << RXC0))));
    return UDR0;
}

int USART_TimedReceive() {
    uint16_t counter = 10000;
    while ((!(UCSR0A & (1 << RXC0))) && counter-- != 0);
    if (counter)
        return UDR0;
    else
        return -1;
}


// Envia uma string
void USART_Print(const char* s) {
    while (*s) USART_Transmit(*s++);
}


// Função simples de backspace: move cursor, apaga e volta
void USART_backspace() {
    USART_Transmit('\b');
    USART_Transmit(' ');
    USART_Transmit('\b');
}