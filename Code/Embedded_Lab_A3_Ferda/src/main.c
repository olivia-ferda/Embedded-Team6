#include <asf.h>
#include <utilities.h>
#include <conf_board.h>
#include <conf_clock.h>
#include <string.h>


int main (void)
{
	board_init();
	sysclk_init();
	configure_console();
	calculate_on_off_times();
	configure_tc();

	SysTick_Config(sysclk_get_cpu_hz()/500000);
	
	//Set priority of the SysTick Handler
	NVIC_SetPriority(SysTick_IRQn,0);
	
	//motor 1
	ioport_set_pin_dir(EXT2_PIN_9,IOPORT_DIR_OUTPUT);//set motor direction A2 as output
	ioport_set_pin_dir(EXT2_PIN_3,IOPORT_DIR_OUTPUT);// set motor direction A1 as output
	
	//motor 2
	ioport_set_pin_dir(EXT2_PIN_6,IOPORT_DIR_OUTPUT);//set motor direction B2 as output
	ioport_set_pin_dir(EXT2_PIN_10,IOPORT_DIR_OUTPUT);// set motor direction B1 as output
	
	ioport_set_pin_dir(EXT1_PIN_5,IOPORT_DIR_OUTPUT);//PWM for motor 1
	ioport_set_pin_dir(EXT2_PIN_5,IOPORT_DIR_OUTPUT);//PWM for motor 2
	printf("hi");
	while(1) {
		int n = 0;
		float avg = 0;
		float avg2 = 0;
		while(n < 5){
			mdelay(25000);
			
			ioport_set_pin_dir(EXT1_PIN_4, IOPORT_DIR_OUTPUT);//trigger
			
			ioport_set_pin_level(EXT1_PIN_4,0);
			
			ioport_set_pin_level(EXT1_PIN_4,1);		// send out chirp from sensor
			mdelay(3);
			
			ioport_set_pin_level(EXT1_PIN_4,0);
			
			ioport_set_pin_dir(EXT1_PIN_4, IOPORT_DIR_INPUT);//echo
			
			while(ioport_get_pin_level(EXT1_PIN_4)==0){
				
			}
			startTime = ticks;
			
			while(ioport_get_pin_level(EXT1_PIN_4)==1){
				
			}
			stopTime = ticks;

			//distance calculations:
			difference = stopTime - startTime;
			
			rangeCm = (float)(difference * 343.0f*100.0f)/(1000000.0f);
			
			avg += rangeCm;
			
			n++;
		}
		
		mdelay(100);
		avg = avg/5.0f;
		printf("avg: %.2f \r\n", avg);
		
		//motor logic based off of distance calculations:
		if((avg) <= 20)
		{
			//CCW:
			ioport_set_pin_level(EXT2_PIN_3,0);//low A1
			ioport_set_pin_level(EXT2_PIN_9,1);//high A2
			
			//CW:
			ioport_set_pin_level(EXT2_PIN_6,1);//low B2 black
			ioport_set_pin_level(EXT2_PIN_10,0);//high B1 red
			
			
			duty_cycle = 20;
			calculate_on_off_times();
			mdelay(10);
		}
		
		else if ((avg) <= 50){
			//CW
			ioport_set_pin_level(EXT2_PIN_3,1);//high
			ioport_set_pin_level(EXT2_PIN_9,0);//low
			
			//CCW:
			ioport_set_pin_level(EXT2_PIN_6,0);//low
			ioport_set_pin_level(EXT2_PIN_10,1);//high
			
			duty_cycle = 0;
			calculate_on_off_times();
			mdelay(10);
		}
		
		else if ((avg <= 100)){
			//CW
			ioport_set_pin_level(EXT2_PIN_3,1);//high
			ioport_set_pin_level(EXT2_PIN_9,0);//low
			
			//CCW
			ioport_set_pin_level(EXT2_PIN_6,0);//low
			ioport_set_pin_level(EXT2_PIN_10,1);//high
			
			duty_cycle = 17;
			calculate_on_off_times();
			mdelay(10);
		}
		
		else if ((avg <= 150)){
			//CW
			ioport_set_pin_level(EXT2_PIN_3,1);//high
			ioport_set_pin_level(EXT2_PIN_9,0);//low
			
			//CCW
			ioport_set_pin_level(EXT2_PIN_6,0);//low
			ioport_set_pin_level(EXT2_PIN_10,1);//high
			
			duty_cycle = 34;
			calculate_on_off_times();
			mdelay(10);
		}
		else if ((avg <= 200)){
			//CW
			ioport_set_pin_level(EXT2_PIN_3,1);//high
			ioport_set_pin_level(EXT2_PIN_9,0);//low
			
			//CCW
			ioport_set_pin_level(EXT2_PIN_6,0);//low
			ioport_set_pin_level(EXT2_PIN_10,1);//high
			
			duty_cycle = 51;
			calculate_on_off_times();
			mdelay(10);
		}
		else if ((avg <= 250)){
			//CW
			ioport_set_pin_level(EXT2_PIN_3,1);//high
			ioport_set_pin_level(EXT2_PIN_9,0);//low
			
			//CCW
			ioport_set_pin_level(EXT2_PIN_6,0);//low
			ioport_set_pin_level(EXT2_PIN_10,1);//high
			
			duty_cycle = 68;
			calculate_on_off_times();
			mdelay(10);
		}
		else if ((avg <= 350)){
			//CW
			ioport_set_pin_level(EXT2_PIN_3,1);//high
			ioport_set_pin_level(EXT2_PIN_9,0);//low
			
			//CCW
			ioport_set_pin_level(EXT2_PIN_6,0);//low
			ioport_set_pin_level(EXT2_PIN_10,1);//high
			
			duty_cycle = 85;
			calculate_on_off_times();
			mdelay(10);
		}

	}

	return 0;
}