#include "../inc/LCD.h"
#include <avr/io.h>
#include <util/delay.h>

// Mapeamento de pinos
#define LCD_RS 2  // PD2 -> D2
#define LCD_EN 3  // PD3 -> D3
#define LCD_D4 4  // PD4 -> D4
#define LCD_D5 5  // PD5 -> D5
#define LCD_D6 6  // PD6 -> D6
#define LCD_D7 7  // PD7 -> D7

#define LCD_PORT PORTD
#define LCD_DDR DDRD
#define MAX_COL 15
#define MAX_ROW 1

// Funções internas: declara-se primeiro para não haver conflito
static void LCD_EnablePulse(void);
static void LCD_Send4Bits(uint8_t data);
static void LCD_Command(uint8_t cmd);

uint8_t cursorCol = 0;
uint8_t cursorRow = 0;
char charVec[MAX_ROW + 1][MAX_COL + 1];             // Vetor de Caracteres


void LCD_Init(void) {
    // Configura pinos como saída
    LCD_DDR |= (1<<LCD_RS)|(1<<LCD_EN)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);
    _delay_ms(40); // Delay inicial

    // Inicialização em modo 4 bits
    LCD_Send4Bits(0x03);
    _delay_ms(5);
    LCD_Send4Bits(0x03);
    _delay_us(150);
    LCD_Send4Bits(0x03);
    _delay_us(150);
    LCD_Send4Bits(0x02); // Modo 4 bits
    _delay_us(150);

    LCD_Command(0x29); // 4 bits, 2 linhas, 5x8
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x06); // Incrementa cursor
    LCD_Clear();

    // Inicializando vetor de chars
    for (int i = 0; i <= MAX_ROW; i++){
        for (int j = 0; j <= MAX_COL; j++){
            charVec[i][j] = '\0';
        }
    }
}


bool LCD_PrintChar(char c) {
    if (cursorCol == MAX_COL + 1 && cursorRow == MAX_ROW)
        return 0;
    LCD_PORT |= (1<<LCD_RS); // RS=1 para dado
    LCD_Send4Bits(c >> 4);
    LCD_Send4Bits(c & 0x0F);
    
    charVec[cursorRow][cursorCol] = c;

    if (cursorCol == 15 && cursorRow == 0){
        cursorCol = 0;
        cursorRow++;
        LCD_SetCursor(cursorRow, cursorCol);
    }

    cursorCol++;
    _delay_us(50);
    return 1;
}


bool LCD_Print(const char *str) {
    bool loopPermit = true;
    while(*str && loopPermit){
        LCD_PrintChar(*str++);
    }

    return loopPermit;
}


void LCD_Println(const char *str) {
  LCD_Print(str);
  LCD_SetCursor(0, 1);
}


void LCD_Clear(void) {
    LCD_Command(0x01);
    _delay_ms(2);
}


void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row==0?0x00:0x40) + col;
    LCD_Command(0x80 | addr);
}


void LCD_CursorShow(bool state, bool blink){
    LCD_Command(0b00001100 | (state * (1 << 1)) | (blink * (1 << 0)));
    _delay_ms(2);
}



// Sequências de Escape
void LCD_Backspace(){
    if (cursorCol == 0 && cursorRow == 0)
        return;
    if (cursorRow != 0 && cursorCol == 0){
        cursorRow--;
        cursorCol = MAX_COL;
        LCD_SetCursor(cursorRow, cursorCol);
        LCD_PrintChar(' ');
        cursorRow--;
        cursorCol = MAX_COL;
        LCD_SetCursor(cursorRow, cursorCol);
        return;
    }

    LCD_SetCursor(cursorRow, --cursorCol);
    LCD_PrintChar(' ');
    LCD_SetCursor(cursorRow, --cursorCol);
}


void LCD_Del(){
    LCD_PrintChar('A');
}


void LCD_Esc(){
    LCD_PrintChar('E');
}


void LCD_ArrowUp(){
    ;;
}


void LCD_ArrowDown(){
    ;;
}


void LCD_Enter(){
    ;;
}

void LCD_ArrowLeft(){
    if (cursorRow == 0 && cursorCol == 0){
        return;
    } else if (cursorCol == 0 && cursorRow == MAX_ROW){
        cursorCol = MAX_COL;
        cursorRow--;
    } else {
        cursorCol--;
    }
    LCD_SetCursor(cursorRow, cursorCol);
}


void LCD_ArrowRight(){
    if (cursorRow == MAX_ROW && cursorCol == MAX_COL){
        return;
    } else if (cursorCol == MAX_COL && cursorRow == 0){
        cursorCol = 0;
        cursorRow++;
    } else {
        cursorCol++;
    }
    LCD_SetCursor(cursorRow, cursorCol);
}




// --- Funções internas ---
static void LCD_Send4Bits(uint8_t data) {
    // Limpa pinos D4-D7
    LCD_PORT &= ~((1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7));

    if(data & 0x01) LCD_PORT |= (1<<LCD_D4);
    if(data & 0x02) LCD_PORT |= (1<<LCD_D5);
    if(data & 0x04) LCD_PORT |= (1<<LCD_D6);
    if(data & 0x08) LCD_PORT |= (1<<LCD_D7);

    LCD_EnablePulse();
}


static void LCD_Command(uint8_t cmd) {
    LCD_PORT &= ~(1<<LCD_RS); // RS=0 para comando
    LCD_Send4Bits(cmd >> 4);
    LCD_Send4Bits(cmd & 0x0F);
    _delay_us(50);
}


static void LCD_EnablePulse(void) {
    LCD_PORT |= (1<<LCD_EN);
    _delay_us(1);
    LCD_PORT &= ~(1<<LCD_EN);
    _delay_us(50);
}
