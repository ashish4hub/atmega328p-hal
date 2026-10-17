# ATmega328P HAL

## Overview

This project provides a collection of reusable drivers for the ATmega328P.

Instead of directly manipulating MCU registers throughout the application, peripheral access is organized into dedicated drivers.

The general architecture is:
```
       Application Layer
                |
                |
                v
              Drivers
                |
                |
                v
        Hardware Registers
 ```

*The project also contains a UART-based command-line interface for interacting with the firmware during development and hardware testing.*

## Project Structure
```
    atmega328p-hal
        |
        |----> driver/
        |        |
        |        |-->adc/
        |        |    |--> adc.c
        |        |    |--> adc.h
        |        |
        |        |--> icu/
        |        |     |--> icu.c
        |        |     |--> icu.h
        |        |
        |        |--> pwm/
        |        |     |--> pwm.c
        |        |     |--> pwm.h
        |        |
        |        |--> spi/
        |        |     |--> spi.c
        |        |     |--> spi.h
        |        |
        |        |--> timer/
        |        |      |--> timer.c
        |        |      |--> timer.h
        |        |
        |        |--> uart/
        |        |      |--> uart.c
        |        |      |--> uart.h
        |        |
        |        |--> gpio/
        |        |      |--> gpio.c
        |        |      |--> gpio.h
        |        |
        |        |--> servo/
        |        |      |--> servo.c
        |        |      |--> servo.h
        |
        |
        |----> src/
        |----> CLI/
        |----> main.c
```

## Target Microcontroller
- **ATmega328P**

## Drivers
`drivers/`

The project currently contains hardware peripheral drivers for:
- ADC (Analog to Digital converter)
- PWM (Pulse Width Modultion)
- Timer
- GPIO
- UART
- SPI
- Servo (Servo Motor)

# ADC Driver
The ADC driver provides an interface to the ATmega328P's **10-bit ADC**.
The driver handles ADC configuration, channel selection, conversion triggering, and result retrieval.
## Public API
The ADC interface provides functionality for:
- `ADC_init()`
- `ADC_start()`
- `ADC_done()`
- `ADC_get_result()`
- `ADC_eanble()`
- `ADC_disable()`

## ADC_init()
Initialize ADC hardware with desired Reference Voltage, Prescaler and Mode [Interrupt/Polling].

Typical Usage:
```c
/* ADC Configuration */
    ADC_config_t adc_config = {
        .reference = ADC_REF_VCC,
        .prescaler = ADC_PS_128,
        .mode = ADC_INTERRUPT
    };

    ADC_init(&adc_config);    // ADC initialization

```

## ADC_start()
Starts an `ADC Conversion` on the selected `Channel`. It selects the requested ADC channel and triggers a conversion.

Typical usage:
```c
ADC_start(ADC_CH1);
```

## ADC_done()
Checks whether the current ADC conversion has completed by monitoring the ADC start conversion (`ADSC`) bit. `ADSC` remains set while conversion is in progress and clears when conversion completes.

**Returns:**
- 1 - When conversion complete
- 0 - Conversion still in progress OR not yet started

Typical Usage:
```c
if(ADC_done())
{
    uint16_t result = ADC_get_result();
}
```

## ADC_get_result()
Reads the completed ADC conversion result from the `ADCL` and `ADCH` register.

**Returns:** 10-bit ADC conversion result (0-1023).

Typical Usage:
```c
uint16_t adc_result = ADC_get_result();
```
## ADC_enable()
Enables the ADC peripheral by setting the ADC Enable (`ADEN`) bit in the `ADCSRA` register.

## ADC_disable()
Disables the ADC peripheral by clearing the ADC Enable (`ADEN`) bit in the `ADCSRA` register.

### Typical Implementation 
```
ADC_init(&config);
ADC_start(ADC_CH0);
while (!ADC_done());
uint16_t value = ADC_get_result();
```
### ADC configuration

```
The driver supports configuration of parameters such as:
- Voltage reference
- ADC prescaler
- Conversion mode

The driver is designed so that application code does not need to directly manipulate ADC control registers for normal operation.
```
### ADC Channel Mapping
---
| ADC Channel | ATmega38P Pin | Arduino UNO Pin |
|-------------|---------------|-----------------|
|ADC_CH0      | PC0 / Pin 23  | A0              |
|ADC_CH1      | PC1 / Pin 24  | A1              |
|ADC_CH2      | PC2 / Pin 25  | A2              |
|ADC_CH3      | PC3 / Pin 26  | A3              |
|ADC_CH4      | PC4 / Pin 27  | A4              |
|ADC_CH5      | PC5 / Pin 28  | A5              |


# PWM Driver
The PWM driver provides configurable PWM generation using the ATmega328P timer peripherals.
It is intended for applications such as:
- Motor control
- LED brightness control
- Servo-related signals
- General-purpose waveform generation
The driver supports PWM configuration through the hardware timers rather than implementing PWM entirely in software.
## Public APIs
- `pwm_init()`
- `pwm_set()`
- `pwm_disable()`
- `pwm_start()`

## pwm_init()
```C
void pwm_init(uint32_t freq, pwm_timer_t timer);
```
Initializes the selected `Timer` for `PWM` at the requested `Frequency`.

**Working:** The `driver`selects suitable `Prescaler` and calculate the timer's `TOP` value so that the requested frequency can be generated. `TIMER1` uses `ICR1` as TOP, while `TIMER2` uses `OCR2A`.

**Parameters:**
| Parameter | Description |
|-----------|-------------|
| `freq`    | Desired PWM frequency in hz |
| `timer`   | `pwm_timer1` or `pwm_timer2` |

Typical usage:
``` C
pwm_init(1000, pwm_TIMER1)
```
This configures `Timer1` for a 1kHz `PWM` signal.

## pwm_set()
```C
void pwm_set(pwm_channel_t ch, uint32_t duty_percent);
```
Sets duty cycle for the selected `PWM` channel.

**Working:** The requested percentage is converted into the corresponding Output Compare Register (`OCR`) value using the Timers's calculated `TOP` value. *Values over 100% are clamped to 100%*.

**Parameters:**
| Parameter | Description |
|-----------|-------------|
|`ch`       | PWM output channel |
| `duty_percent` | Duty cycle from 0 to 100% |

Typical usage:
```C
pwm_set(pwm_CH1A,50);
```
This produces approx a 50% duty-cycle on `PB1 / OC1A`.

## pwm_disable()
```C
void pwm_disable(pwm_channel_t ch);
```
Disables PWM output on the selected channel and drives the corresponding pin `low`.

## pwm_start()
```C
void pwm_start(pwm_channel_t ch);
```
Starts or re-enables PWM output on the selected channel.

**PWM Channel Mapping:**
| PWM Channel | ATmega328P |
|-------------|------------|
| `pwm_CH1A1` | `PB1`      |
| `pwm_CH1B`  | `PB2`      |
| `pwm_CH2B`  | `PD3`      |
---


# timebase Driver

The `timebase` driver provides a `1 ms` system time base using `Timer0` in `CTC` mode.
`Timer0` is configured with a `64x` prescaler and `OCR0A = 249`, producing a compare match interrupt every 1 ms at a `16 MHz` CPU clock. Each interrupt increments a global `tick` counter.

## APIs
- `timebase_init()`
- `timebase_get_ticks()`
- `nb_wait_ms()`

## timebase_init()
Initialize `Timer0` in `CTC` mode.

Typical Usage:
```c
timebase_init();
```

## get_ticks()

Returns current `ticks (Time count)`. `1 tick = 1 MS`.

Typical Usage
```C
uint32_t time = get_ticks();
```

## nb_wait_ms()
Checks whether the specified time interval has elapsed without blocking the program execution.
**Parameters:**
| Parameter | Type | Description |
|-----------|------|------------|
| `*prv_time` | uint32_t * | Pointer to the timestamp of the previous execution. Updated when interval expires. |
| `time`    | uint32_t | Required time interval in milliseconds. |
**Returns:**
```
- 1  --> When requested interval has elapsed.
- 0  --> Interval has not elapsed yet.
```

Typical usage:
```C
if(nb_wait_ms(&current_time,500)){
    ...
}
```
This creates a `Non-blocking` delay of `500 MS` without blocking the `CPU`.

# GPIO Driver
The `gpio` drivers provides and interface to the digital input/output pins of ATmega328P.
The Register and bits corresponding to the selected pin are set according to the mode of opeartion by driver, making it simple and easy to use.

## Public API
- `gpio_init()`
- `gpio_write()`
- `gpio_read`

## Pin Mapping

| Pin number | ATmega328P Pin |
|------------|----------------|
| 0          | PD0 |
| 1          | PD1 |
| 2          | PD2 |
| 3          | PD3 |
| 4          | PD4 |
| 5          | PD5 |
| 6          | PD6 |
| 7          | PD7 |
| 8          | PB0 |
| 9          | PB1 |
| 10         | PB2 |
| 11         | PB3 |
| 12         | PB4 |
| 13         | PB5 |

## gpio_init()
Initialize the desired I/O pin in desired direction Input/Output.

Parameters:
| Parameter | Type | Description |
|-----------|------|-------------|
| `pin`     |uint8_t | Pin number according t the pin map |
| `direction` |GPIO_dir_t| Initialization direction of the pin (Input/Output)|

**Direction:**
The driver uses a internal direction type enumerator for direction configuration of a pin.
- `GPIO_INPUT` ---> Input
- `GPIO_OUTPUT` ---> Output

*Directions of the pin are C enumerator and evaluates to a integer value:*

- `GPIO_INPUT` Evaluates to 0
- `GPIO_OUTPUT` Evaluates to 1

**Typical Usage:**
```c
gpio_init(5,GPIO_INPUT);
```
## gpio_write()
Write the initialized pin HIGH/LOW. For writing a digital pin HIGH/LOW the driver uses internal state type enumerator.

**Parameters:**
| Parameter | Type | Description |
|-----------|------|-------------|
| `pin`     |uint8_t| Pin number |
| `state`   |`Gpio_state_t` | Pin state to write HIGH/LOW |


**Write State:**
- `GPIO_HIGH` ---> Write the initialized pin HIGH
- `GPIO_LOW` ---> Write the initialized pin LOW.

*State enumerator evaluation:*
- `GPIO_HIGH` Evaluate to 1
- `GPIO_LOW` Evaluates to 0

**Typical Usage:**
```c
gpio_write(5,GPIO_HIGH);
```
## gpio_read()
Read a digital pin state HIGH/LOW which is initialized in `INPUT` direction.

**Returns:**
- `GPIO_HIGH` ---> When pin is high
- `GPIO_LOW` ---> When pin is low

*Since `GPIO_HIGH` and`GPIO_LOW` are C enumerator type, they evaluate to an integer value.*

- `GPIO_HIGH` Evaluates to 1
- `GPIO_LOW` Evaluates to 0

**Typical Usage:**
```c
gpio_read(2);
```
# Servo Driver
The servo driver is built using ATmega328P TIMER2 peripheral.

## Features
- Configures TIMER2 peripheral in CTC Mode with `8x` prescaler.
- Uses hardware interrupt to produce precise timeframe for pulse generation.
- Supports all GPIO pins of ATmega328P.
- Easy and simple interface
- Non blocking and precise

## API
- `servo_init()`
- `servo_move_step()`

## servo_init()
Initialize the `Timer2` peripheral with required configurations.

- `Timer2` configured in CTC mode.
- Precscaler 8 is configured

## servo_move_step()
Responsible for providing the pin and pulse duration in the form of steps to the `ISR`.

Pulse generation is handles by the hardware interrupt. With current configuration of the TIMER2 peripheral, the interrupt fires every `0.1ms` and updates a counter variable.

The step value required by the user is compared against the counter variable and and the specified I/O pin outputs the required pulse.

**Parameters:**
| Parameter | Type | Description |
|-----------|------|-------------|
| pin       |uint8_t| Any I/O pin can be used according to the [gpio driver](driver/gpio/gpio.c) |
| `step`    |uint8_t| Number of steps for pulse generation |

**step:**
The `servo` driver internally uses a step value instead of pulse duration for generating pulse.
- `1 step = 0.1 ms pulse`
- Current Architecture supports step values upto 200 (20ms) because a typical servo motor needs pulse every 20ms.
- 200 steps = 20 ms


# UART Driver
The UART driver provides serial communication using the ATmega328P USART peripheral.
The driver is designed around interrupt-driven communication and buffering rather than relying only on blocking polling operations.
The UART implementation includes:
- TX buffering
- RX buffering
- Interrupt-based transmission/reception
- Configurable baud rate
- Serial data handling Conceptually:
```
    Application
        |
        |
        v 
    UART API
        |
        |
        v
    TX/RX Buffers
        |
        |
        v
    USART Peripheral
        |
        |
        v
    Serial Interface
```
This allows the UART to be used by higher-level components such as the CLI.
## APIs
| APIs | Purpose |
|------|---------|
| `USART_init()` | Initializes `USART` hardware with requested `baud rate`. |
| `USART_print()` | Transmits a null-terminated string. |
| `USART_printIN` | Transmit an integer |
| `USART_rx_avil()` | Checks if `received` data is available |
| `USART_get_data()` | Retrieves one received character |
| `USART_TX_byte` | Adss one byte to the TX buffer for transmission |
## USART_init()
Calculates the USART baud-rate register value from `F_CPU` and the requested baud rate, enables the transmitter and receiver, and enables the interrupt. The driver configures the USART for `8-bit` character transmission.

```C
void USART_init(uint32_t BAUD);
```
**Parameter:**
|Parameter | Type | Description |
|----------|------|-------------|
| `BAUD`   | uint32_t | Desired communication baud rate. |

Typical usage:
```C
USART_init(9600);
```
## USART_print()
Transmits a `null-terminated` string through `USART`.
```C
void USART_print(const char *str);
```
**Parameter:**
| Parameter | Type | Description |
|-----------|------|-------------|
| `str`     |`const char *`| Pointer to the null-terminated string. |

Typical usage:
```C
USART_print("Hello world\n");
```
*`USART_print` does not directly manipulate the USART hardware for every character. It uses the lower level internal `USART_TX_byte()` API.*

## USART_printIN()
```C
void USART_printIN(int num);
```
Converts an integer into its decimal character representation and transmits it through the USART.
Handles zero and negative values, converts the individual decimal digits into characters then sends them through `USART_TX_byte`.

**Parameters:**
| Parameter | Type | Description |
|-----------|------|-------------|
| num       | int  | Integer value to transmit |

Typical usage:
```C
int distance = 50;
USART_ptintIN(distance);
```
## USART_rx_avil()
Checks whether unread data is avilable in the `RX buffer`. compares the RX `head` and `tail` indexes of the `circular buffer`.

**Returns:**
- 1  --> Data is available.
- 0  --> RX buffer is empty.

Typical usage:
```C
if(USART_rx_avil())
{
    char data = USART_get_data();
}
```
This function allows the application to check for incoming data without blocking while waiting for a character.

## USAR_get_data()
```C
char USART_get_data(void);
```
Retrives the next unread character from the RX buffer. reads the character at the current RX tail position and advances the tail index.

**Returns:**
- Received character when data is availble
- 0 when RX buffer is empty

### Interrupt-Driven Reception
*When a byte arrives the USART RX interrupt is triggered. The `ISR` stores the received byte in the RX buffer and advances the buffer head. If the buffer is full the incoming byte is discarded rather than overwriting the unread data.*

### Interrupt-Driven Transmission
*When the TX buffer becomes empty, the driver disables the Data register Empty interrupt. When data is avilable, the ISR continues loading bytes into `UDR0`.*

### Buffer Configuration
*The driver defines:*
```C
#define Tx_buffer_size 64
#define Rx_buffer_size 64
#define line_size 32
```
### Example:
```C
USART_init(9600);
USART_print("Hello world\n");
USART_printIN(123);

while(1)
{
    if(USART_rx_avail())
    {
        char data = USART_get_data();
    }
     // Process received character
}
```

---
# SPI Driver
*The `SPI` driver provides a hardware abstraction for ATmega328P's `Serial Peripheral Interface in Master mode.*
## SPI Pin Mapping
| SPI signal | ATmega328P | Direction |
|------------|------------|-----------|
| `MOSI`     | PB3        | Output    |
|`MISO`      | PB4        | Input |
| `SS`       | PB2        | Output |
|`SCK`       | PB5        | Output |

*These Pin assignment and the current SPI configuration are implemnted dirctly in `spi.c`.*
## Public APIs
|API | Purpose |
|----|---------|
|`SPI_init()` | Initialize the SPI peripheral as master |
| `SPI_transfer()` | Sends and revceives one byte through SPI |

## SPI_init()
```C
void_SPI_init(void);
```
- Configures `MOSI`, `SS`, `SCK` as output.
- Configures  `MISO` as an input.
- Enables the SPI peripheral.
- Selects Master mode.
- Configures the SPI clock as `F_CPU/64`.
- Configures SPI mode 0 (`CPOL = 0`, `CPHA = 0`).
*Thses settings are implemented through `DDRB` and `SPCR` registers.*

## SPI_transfer()
*Transfers one byte through SPI peripheral and returns the byte received during the same transfer.*

**Parameters**
| Parameter | Type | Description |
|-----------|------|-------------|
| data      | uint8_t | Byte to transmit |
**Returns**
```C
uint8_t
```
*The byte received from SPI slave.*
## Current Limitations
*The current SPI driver is intentionally minimal.*


# ICU Driver
The `ICU` driver uses `ATmega328P` `Timer1` `Input Capture Unit` to record the timer value when and external signal edge occures.
This allows the firmware to measure `time intervals` between signal edges which can be used for pulse-widht, frequency and sensor timing measurement.

**Typical Application**
- Ultrasonic sensor echo timing
- Pulse-width measurement
- Frequency measurement
- Period measurement

## Public APIs
| APIs | Purpose |
|------|---------|
| `ICU_init()` | Initializes Timer1 Input Capture. |
| `ICU_set_edge()` | Change the Capture Edge. |
| `ICU_done()` | Checks whether a capture event occured. |
| `ICU_get_capture()` | Returns the captured Timer1 value. |
| `ICU_clear()` | Clears the capture status |
| `ICU_stop()` | Stops ICU capture and disables its interrupt |

## ICU_init()
Initailizes the Timer1 Input Capture peripheral using the supplied configuration.
Configures the capture edge, input noise canceler and Timer1 prescaler. It also clears any previous flag and enables the Timer1 Input Capture interrupt.

**Parameter**
| Parameter | Type | Description |
|-----------|------|-------------|
| `config`     | const ICU_config_t * | Pointer to ICU configuration |

Typical usage:
```C
ICU_config_t config = {
    .edge = ICU_rising,
    .prescaler = ICU_ps_8,
    .noise = ICU_noise_on
};

ICU_init(&config);
```
## ICU_set_edge()
Changes the signal edge that triggers an input capture event.

**Parameter**
| Parameter | Type | Description |
|-----------|------|-------------|
| edge      | ICU_edge_t | `ICU_rising` or `ICU_falling`|

Typical usage:
```C
ICU_set_edge(ICU_falling);
```
*The driver updates the `ICES1` bit in the Timer1 control register to select the required edge.*

## ICU_done()
```C
uint8_t ICU_done(void);
```
Checks whether a new input capture event has occured.

**Returns**
- 1  --> New capture value is available
- 0  --> No new capture value

Typical usage:
```C
if(ICU_done())
{
    uint16_t capture = ICU_get_capture();
}
```
## ICU_get_capture()
```C
uint16_t ICU_get_capture(void);
```
Returns the Timer1 value captured during the most recent input capture event.

**Returns**
`16-bit` Timer1 capture value.

## ICU_clear()
```C
void ICU_clear(void);
```
*Clears the current ICU capture status.*

## ICU_done()
```C
void ICU_stop(void);
```
*Stops Timer1 counting and disables the Timer1 input capture interrupt.*

## Example Implementation
```C
ICU_config_t config = {
    .edge = ICU_rising,
    .prescaler = ICU_ps_8,
    .noise = ICU_noise_on
};

ICU_init(&config);

while(1)
{
    if(ICU_done())
    {
        uint16_t capture = ICU_get_capture();

        ICU_clear();
    }
}
```

# Testing `testing/`
The `testing` directory contain a simple application of the drivers for testing purpose.
The drivers are tested and validated on ATmega328P hardware setup.

*The application, services and CLI are only developed for testing of the drivers.*

## Service Layer `src/`
The service layer `src` is built on top of the drivers to be used for different application for testing on hardware.

Service layer is intended for:
- Implementation of the drivers in applications.
- Testing and validation of the drivers in real applications.

## CLI `CLI/`
UART based command line interface is built for testing and validation of the drivers being used in application on hardware.

The CLI is intended for:
- Peripheral testing
- Hardware experimentation
- Runtime interaction
- Driver verification
- Debugging


# Development Workflow
## Testing and Verification
Driver development is performed through hardware experiments and peripheral-specific tests.
Examples include:
### UART
Communication can be verified using a serial terminal.
### PWM
PWM output can be observed using measurement equipment such as a logic analyzer or oscilloscope.
### ADC
ADC readings can be compared against known input voltages.
### timebase / ICU
timebase and input-capture functionality can be verified by measuring signal timing and captured values.
### GPIO
Can be verified by simple led blink
### Servo
A logic analyzer or tools like PulseView can be used to analyse the pulse/signal output.

---

## Design Philosophy
The project intentionally avoids placing peripheral register manipulation directly throughout application code.
For example, application code should be able to request an ADC conversion through the ADC driver instead of manually configuring every ADC register each time.
This makes the firmware:
- Easier to reuse
- Easier to understand
- Easier to extend
- Easier to test
- Less dependent on application-specific register manipulation

## Engineering Concepts Practiced
This project focuses on practical embedded firmware concepts including:
- Embedded C
- Pointers and memory-mapped registers
- MCU peripheral configuration
- Interrupts
- Timers and counters
- PWM
- ADC
- UART
- SPI
- Input Capture
- Sensor interfacing
- Hardware abstraction
- Driver architecture
- Modular firmware design
- Cross-compilation
- Make-based build systems
- Hardware debugging

## Why Build a HAL From Scratch?
High-level frameworks make microcontroller development easier, but they can hide much of the hardware underneath.
This project is intentionally built closer to the hardware to understand the relationship between firmware and MCU peripherals.

# Current Status
### The project is an actively evolving firmware framework.

Current areas of development include:
- Peripheral driver implementation
- UART-based CLI
- Hardware testing
- Driver refinement
- Improving modularity and reusability

# License
This project is licensed under the MIT License.
See the [LICENSE](LICENSE) file for details.