/* SPI driver */
/* Hardware abstraction layer for SPI */
/* MCU = ATmega328P */


/*------------------------- PINS --------------------*/
/*
* PB3 ----> MOSI ----> Master Output Slave In
* PB4 ----> MISO ----> Master In Slave Output
* PB2 ----> SS ----> Slave Select
* PB5 ----> SCK ----> Clock
*/

#include "spi.h"

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>


static volatile uint8_t spi_transfer_done = 0;
static uint8_t Operation_mode;
static uint8_t device_mode;

/* Prescaler struct */
typedef struct 
{
    uint8_t spr_bits;
    uint8_t sp2x_bit;
}SPI_ps_map_t;

/* Prescaler Map */
static const SPI_ps_map_t prescaler_table[] =
{
    [SPI_PS_2] = {0b00, 1},
    [SPI_PS_4] = {0b00, 0},
    [SPI_PS_8] = {0b01, 1},
    [SPI_PS_16] = {0b01, 0},
    [SPI_PS_32] = {0b10, 1},
    [SPI_PS_64] = {0b10, 0},
    [SPI_PS_128] = {0b11, 0}
};

/* ==================================================== SPI Initialization ==================================================*/

void SPI_init(SPI_config_t *configuration)
{
    Operation_mode = configuration->spi_operation_mode;                // Mode of operation [Interrupt/polling]
    device_mode = configuration->spi_device_mode;                     // Device mode [Master/slave]

    /* MISO, MOSI, SS, SCK */
    if(device_mode == MASTER_MODE)
    {
        DDRB |= (1 << PB5)
              | (1 << PB3)
              | (1 << PB2);
        
        DDRB &= ~(1 << PB4);
    }
    else if(device_mode == SLAVE_MODE)
    {
        DDRB &= ~((1 << PB5) | (1 << PB3) | (1 << PB2));
        DDRB |= (1 << PB4); 
    }

    /*--------------------------------------- Configuration ------------------------------------*/
    uint8_t spcr_config = 0;

    spcr_config |= (1 << SPE);                                                 // Enable SPI

    spcr_config |= (configuration->spi_operation_mode << SPIE);              // Interrupt Enable

    spcr_config |= (configuration->spi_trans_data_odr << DORD);           // Data transmission order LSB first or MSB first

    spcr_config |= (configuration->spi_device_mode << MSTR);            // SPI device mode [Master/Slave]

    spcr_config |= (configuration->spi_clock_polarity << CPOL);       // SPI clock polarity

    spcr_config |= (configuration->spi_clock_phase << CPHA);        // SPI clock phase

    /*------------------------------------------- Prescaler -------------------------------------*/
    SPI_ps_map_t selected_prescaler = prescaler_table[configuration->spi_sck_prescale];

    uint8_t spr_bits = selected_prescaler.spr_bits;

    spcr_config &= ~((1 << SPR1) | (1 << SPR0));
    spcr_config |= (spr_bits & 0x03) << SPR0;

    /*----------------------------------------- Register ------------------------------------------*/
    SPSR &= ~(1 << SPI2X);
    SPSR |= (selected_prescaler.sp2x_bit << SPI2X);

    SPCR = spcr_config;
}

/*=================================================== SPI Data Transmission Function ==========================================*/
uint8_t SPI_transmit(uint8_t data)
{
    SPDR = data;

    while(!(SPSR & (1 << SPIF)))
    return SPDR;
}

/* ISR Function */
ISR(SPI_STC_vect)
{
    spi_transfer_done = 1;
}
