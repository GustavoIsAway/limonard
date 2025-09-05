###############################################################################
# Configurações do Projeto
###############################################################################
MCU       = atmega328p
F_CPU     = 16000000UL
PROGRAMMER= arduino
PORT      = /dev/ttyUSB0
BAUD      = 115200

CC        = avr-gcc
OBJCOPY   = avr-objcopy
AVRDUDE   = avrdude

CFLAGS    = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Iinc
LDFLAGS   = -mmcu=$(MCU)

###############################################################################
# Estrutura de Pastas
###############################################################################
SRC_DIR   = src
INC_DIR   = inc
CFILES_DIR= cfiles
BIN_DIR   = bin

# arquivos fonte
SRC       = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(CFILES_DIR)/*.c)
OBJ       = $(patsubst %.c,$(BIN_DIR)/%.o,$(notdir $(SRC)))

TARGET    = $(BIN_DIR)/program.elf
HEX       = $(BIN_DIR)/program.hex

###############################################################################
# Regras principais
###############################################################################
all: dirs $(HEX)

# compila e linka
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(CFILES_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# garante que as pastas existam
dirs:
	@mkdir -p $(SRC_DIR) $(INC_DIR) $(CFILES_DIR) $(BIN_DIR)

###############################################################################
# Flash no Arduino
###############################################################################
flash: all
	$(AVRDUDE) -V -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

###############################################################################
# Limpeza
###############################################################################
clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all flash clean dirs

