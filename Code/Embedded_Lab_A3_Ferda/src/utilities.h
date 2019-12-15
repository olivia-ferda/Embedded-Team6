

/*
 * utilities.h
 *
 * Created: 9/3/2019 4:42:14 PM
 *  Author: ferda
 */

#include <conf_uart_serial.h>


#include "conf_uart_serial.h"
#include "conf_c42412a_lcdca.h"
#include "core_cm4.h"
#include "c42412a_segmap.h"
#include "twim.h"

#ifndef UTILITIES_H_
#define UTILITIES_H_

volatile int num_detect;
volatile int mdelay_count;
volatile int ticks, ticks2;
volatile bool unit = true;        //default unit true is Celsius
volatile bool Led_status = false;        //lab A5 led
uint8_t  data_buf_tx[2];
uint8_t  data_buf_rx[2];

volatile int duty_cycle = 0;
volatile int on_time = 0;
volatile int off_time = 0;
volatile int tc_period = 0;
float tc_period_f = 0;
float desired_frequency = 2000.0f; //tc 2000
uint32_t tc_a_val = 0;

volatile int startTime, stopTime,startTime2, stopTime2, difference, difference2;
float rangeCm, rangeCm2;

static gpio_pin_callback_t GPIO_callback(void){
}

static void GPIO_setup(void){
    NVIC_SetPriority(EXT1_PIN_5,1);            //set the priority of GPIO
    
    gpio_enable_pin_interrupt (EXT1_PIN_5);        //enable interrupt

    gpio_set_pin_callback (EXT1_PIN_5, GPIO_callback, NVIC_GetPriority(25));        //make a callback func
    
}


void calculate_on_off_times()
{
	static bool prev_stop = false;
	float tc_a_val_f = tc_period_f * ((float)duty_cycle)/100.0f;
	//printf("tc_a_val: %f\r\n", tc_a_val);
	tc_a_val = (uint32_t) tc_a_val_f; //values to write
	on_time = tc_a_val;
	off_time = tc_period - tc_a_val;
	if(prev_stop == true)
	{
		tc_start(TC0, 0);
	}
	if(duty_cycle == 30){
		prev_stop = true;
		//printf("on_time: %d\r\n", on_time);
		//printf("off_time: %d\r\n", off_time);
	}
	if(duty_cycle == 60){
		prev_stop = true;
		//printf("on_time: %d\r\n", on_time);
		//printf("off_time: %d\r\n", off_time);
	}
	if(duty_cycle == 100)
	{
		tc_stop(TC0, 0);
		ioport_set_pin_level(EXT1_PIN_5, true);
		ioport_set_pin_level(EXT2_PIN_5, true);
		prev_stop = true;
	}
	else if(duty_cycle == 0)
	{
		tc_stop(TC0, 0);
		ioport_set_pin_level(EXT1_PIN_5, false);
		ioport_set_pin_level(EXT2_PIN_5, false);
		prev_stop = true;
	}
	//printf("on = %d off = %d\r\n", on_time, off_time);
} 



static void mdelay(uint32_t delay_ticks){
    uint32_t starting_tick;

    starting_tick = ticks;
    while((ticks - starting_tick)< delay_ticks);
}

void SysTick_Handler(void){
    ticks++;
	ticks2++;
    //printf("hi = %d ", ticks);
}

void configure_sys_tick(int ticks_per_second){
    SysTick_Config(sysclk_get_cpu_hz()/ticks_per_second);
}

static void configure_tc(void)
{
       uint32_t ul_tcclks = 1;
       //clock speed
       uint32_t ul_sysclk = sysclk_get_pba_hz();
       //TCO Config
       sysclk_enable_peripheral_clock(TC0);
       
       //Calculate the values you want to use for each timer counter
       tc_period_f = ((float)sysclk_get_pba_hz()/2.0f)/desired_frequency;
       tc_period = (uint32_t) tc_period_f;
       calculate_on_off_times();
       tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG); //3 timer channels option 1
       
       //enable interrupt
       NVIC_EnableIRQ((IRQn_Type) TC00_IRQn);
       tc_enable_interrupt(TC0, 0, TC_IER_CPCS);

       // start timer counter if you want the tc to start right away
       // in my case, I don't want the timer counter to start until
       // i get user input
       //     tc_start(TC0, 0);


}

//interrupt handler for each task
void TC00_Handler(void) //provided and runs every time interrupt occurs (stops main then returns to same spot)
{
    static bool turn_on = true;
    tc_get_status(TC0, 0);
    tc_stop(TC0, 0);
    if(turn_on)
    {
        ioport_set_pin_level(EXT1_PIN_5, true);
		ioport_set_pin_level(EXT2_PIN_5, true);
        tc_write_rc(TC0, 0, on_time);
    }

    else
    {
        ioport_set_pin_level(EXT1_PIN_5, false);
		ioport_set_pin_level(EXT2_PIN_5, false);
        tc_write_rc(TC0, 0, off_time);
    }

    turn_on = !turn_on;
    tc_start(TC0, 0);

}

static void configure_console(void)
{
const usart_serial_options_t uart_serial_options =
{
.baudrate = CONF_UART_BAUDRATE,
.charlength = CONF_UART_CHAR_LENGTH,
.paritytype = CONF_UART_PARITY,
.stopbits = CONF_UART_STOP_BITS,
};
/* Configure console. */
stdio_serial_init(CONF_UART, &uart_serial_options);
}



#endif /* UTILITIES_H_ */
