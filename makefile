MCU    = atmega328p
F_CPU  = 16000000UL
CC     = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -Os -Wall -DF_CPU=$(F_CPU)

TARGET = main
SRC    = main.c uart/uart.c timer/timer.c
# This converts the .c list into a .o list
OBJ    = $(SRC:.c=.o)

all: $(TARGET).hex

# Link objects to create the ELF
$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Convert ELF to HEX for flashing
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Compile C files into Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET).elf $(TARGET).hex

.PHONY: all clean