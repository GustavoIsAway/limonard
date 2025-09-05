#include "../inc/input.h"
#include "../inc/USART.h"
#include "../inc/LCD.h"
#include <stdint.h>
#include <stdbool.h>

#define ESCAPE_CODES_SIZE 6


typedef struct {
  uint8_t seq[4];
  uint8_t len;
} EscapeCode;


EscapeCode escapeCodes[] = {
  {{0x5B, 0x33, 0x7E, -1}, 3},   // DEL
  {{0x5B, 0x41, -1, -1}, 2},   // ARROW_UP
  {{0x5B, 0x42, -1, -1}, 2},   // ARROW_DOWN
  {{0x5B, 0x44, -1, -1}, 2},   // ARROW_LEFT
  {{0x5B, 0x43, -1, -1}, 2},   // ARROW_RIGHT
  {{-1, -1, -1, -1}, 0},       // ESC
};


typedef enum { 
  DEL,
  ESC,
  ARROW_UP,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  INVALID,
} ANSI_ACTION;


ANSI_ACTION treatEscapeCode(void){
  int8_t buffer[4];

  // Tenta ler até 4 bytes
  for (int i = 0; i < 4; i++){
    buffer[i] = USART_TimedReceive();
  }

  // Compara contra todas as sequências conhecidas
  for (int i = 0; i < ESCAPE_CODES_SIZE; i++){
    bool match = true;
    for (int j = 0; j < escapeCodes[i].len; j++){
      if (buffer[j] != escapeCodes[i].seq[j]) {
        match = false;
        break;
      }
    }
    if (match) {
      switch (i) {
        case 0: return DEL;
        case 1: return ARROW_UP;
        case 2: return ARROW_DOWN;
        case 3: return ARROW_LEFT;
        case 4: return ARROW_RIGHT;
        case 5: return ESC;
      }
    }
  }

  return INVALID;
}


void treatInput(char input){
  if (input == 0x1B){              // ESC -> sequência ANSI
    ANSI_ACTION act = treatEscapeCode();
    switch(act){
      case DEL:
        LCD_Del();
        break;

      case ESC:
        LCD_Esc();
        break;

      case ARROW_UP:
        LCD_ArrowUp();
        break;

      case ARROW_DOWN:
        LCD_ArrowDown();
        break;

      case ARROW_LEFT:
        LCD_ArrowLeft();
        break;

      case ARROW_RIGHT:
        LCD_ArrowRight();
        break;


      default:
        return;
    }

  } else if (input == 0x7F){       // Backspace simples
    LCD_Backspace();
  } else if (input == '\n'){
    LCD_Enter();
  } else {
    LCD_PrintChar(input);
  }
}
