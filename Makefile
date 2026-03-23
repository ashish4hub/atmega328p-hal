MCU = atmega328p
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -Os

TARGET = main
SRC = main.c uart_tx.c

all: $(TARGET).hex

$(TARGET).elf:	$(SRC)
	$(CC) $(CFLAGS) -o $(TARGET).elf$(SRC)

$(TARGET).hex:	$(TARGET).elf 
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

clean:
	rm -f $(TARGET).elf $(TARGET).hex