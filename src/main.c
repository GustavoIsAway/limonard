#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "../inc/LCD.h"
#include "../inc/USART.h"
#include "../inc/input.h"

int main() {
  LCD_Init();
  LCD_Clear();
  LCD_CursorShow(true, false);
  USART_Init();


  while(1){
    char entrada = USART_Receive();
    if (entrada)
      treatInput(entrada);
  }
}
