#ifndef SPI_H
#define SPI_H


/* Device Mode */
typedef enum {
    MASTER_MODE = 1,
    SLAVE_MODE = 0
} SPI_dev_mode_t;

/* Operation mode */
typedef enum
{
    SPI_OP_MODE_INTERRUPT = 1,
    SPI_OP_MODE_POLLING = 0
}SPI_op_mode_t;

/* SCK Prescaler table */
typedef enum {
    SPI_PS_2,
    SPI_PS_4,
    SPI_PS_8,
    SPI_PS_16,
    SPI_PS_32,
    SPI_PS_64,
    SPI_PS_128
} SPI_ps_t;

/* Data transmission order */
typedef enum 
{
    SPI_DATA_ODR_LSB = 1,
    SPI_DATA_ODR_MSB = 0
} SPI_data_odr_t;

/* SPI Clock Polarity */
typedef enum
{
    SPI_IDLE_HIGH = 1,
    SPI_IDLE_LOW = 0

} SPI_clck_polarity_t;

/* SPI clock phase */
typedef enum
{
    SPI_LEAD_EDGE_SAMPLE = 0,
    SPI_LEAD_EDGE_SAMPLE = 1

}SPI_clck_phase_t;

/* Configuration Struct */
typedef struct
{
    SPI_dev_mode_t spi_device_mode;
    SPI_ps_t spi_sck_prescale;
    SPI_op_mode_t spi_operation_mode;
    SPI_data_odr_t spi_trans_data_odr;
    SPI_clck_polarity_t spi_clock_polarity;
    SPI_clck_phase_t spi_clock_phase;

} SPI_config_t;

/* SPI Initialization */
void SPI_init(SPI_config_t *configuration);

#endif