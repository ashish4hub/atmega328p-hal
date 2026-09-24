#include "msg.h"


Msg_state_t current_state = Idle;

static uint8_t dist_once = 1;              // Flag for printing distance initial message once
static uint8_t dist_value = 1;            // Flag for printing distance value continiously
static uint8_t moist_once = 1;             // Flag for printing Moisture initial message once
static uint8_t moist_value = 1;           // Flag for printing Moisture value continiously
static uint8_t idle_once = 1;          // Flag for printing IDLE state message once
static uint8_t led_on = 1;
static uint8_t led_off = 1;
static uint8_t ramp_on = 1;
static uint8_t ramp_off = 1;
static uint8_t fade_on = 1;
static uint8_t fade_off = 1;
static uint8_t invalid_flg = 0; 

static uint32_t dist_wait = 0;            // Flag for printing distance values in fixed time intervals
static uint32_t moist_wait = 0;            // Flag for prinitng moisture values in fixed time intervals

static uint32_t dist = 0;             // Store distance value

/* Set state function */
void MSG_set_state(Msg_state_t state){
    current_state = state;
}

/* State based operation */
void MSG_src(void){

    switch (current_state)
    {

/* Idle state */
    case Idle:
     
    // Reset flags in idle state
    dist_once = 1;
    moist_once = 1;
    dist_value = 1;
    moist_value = 1;
    invalid_flg = 1;
    
    // Printing system idle message once
    if(idle_once == 1){
        USART_print("> System in IDLE state!\n");
        idle_once = 0;
    }
    break;

    /* Distance measurement start*/
    case START_DISTANCE:

    if(dist_once == 1){
        USART_print("> Distance mesurement started\n");
        dist_once = 0;
    }
    if(dist_value == 1){
        if(nb_wait_ms(&dist_wait,500)){
            if(HCSR04_done()){
                dist = HCSR04_get_distance();           // Distance value
                USART_print("> Obstacle at: ");
                USART_printIN(dist);
                USART_print(" CM\n"); 
            }
        }
    }
    break;

/* Distance measurement stop */
    case STOP_DISTANCE:
    USART_print("> Distance measurement stopped\n");
    dist_once = 1;
    break;

    /* Moisture measurement start */
    case START_MOIST:
    if(moist_once == 1){
        USART_print("> Moisture detection started\n");
        moist_once = 0;
    }
    if(moist_value == 1){
        if(nb_wait_ms(&moist_wait,500)){
            if(ADC_done()){
                USART_print("> Moisture value: ");
                USART_printIN(MOIST_result());
                USART_print("\n");
            }
        }
    }
    break;

    /* Moisture measurement stop */
    case STOP_MOIST:
    USART_print("> Moisture detection stopped\n");
    moist_once = 1;
    break;

/* LED on */
    case LED_ON:
    if(led_on == 1){
        USART_print("> LED ON\n");
        led_on = 0;
        led_off = 1;
    }
    break;

    /* LED off */
    case LED_OFF:
    if(led_off == 1){
        USART_print("> LED OFF\n");
        led_off = 0;
        led_on = 1;
    }
    break;

/* LED ramp start */
    case RAMP_START:
    if(ramp_on == 1){
        USART_print("> LED ramp started\n");
        ramp_on = 0;
    }
    break;

    /* LED ramp off */
    case RAMP_OFF:
    if(ramp_off == 1){
        USART_print("> LED ramp stopped\n");
        ramp_off = 0;
    }
    break;

    /* LED fade start */
    case FADE_START:
    if(fade_on == 1){
        USART_print("> LED fade started\n");
        fade_on = 0;
    }
    break;

    /* LED fade off */
    case FADE_OFF:
    if(fade_off == 1){
        USART_print("> LED fade stopped\n");
        fade_off = 0;
    }
    break;

/* Operation quit "x" */
    case Quit:
    idle_once = 1;
    current_state = Idle;
    USART_print("\n Operation Quit \n");
    break;

    /* Invalid command */
    case Invalid:
    if(invalid_flg == 1){
        USART_print("Invalid command \n");
        current_state = Idle;
    }
    break;

    default:
        break;
    }
}