#ifndef LCD_H
#define LCD_H
#include <stdint.h>
#include <stdbool.h>


// Inicialização do LCD
void LCD_Init(void);


// Comando e dados
bool LCD_PrintChar(char c);
bool LCD_Print(const char *str);
void LCD_Println(const char *str);
void LCD_CursorShow(bool state, bool blink);
void LCD_Backspace();
void LCD_Enter();


// Sequências de Escape ANSI
void LCD_Del();
void LCD_Esc();
void LCD_ArrowUp();
void LCD_ArrowDown();
void LCD_ArrowLeft();
void LCD_ArrowRight();


// Cursor e posições
void LCD_SetCursor(uint8_t row, uint8_t col);


// Limpar tela
void LCD_Clear(void);


#endif
