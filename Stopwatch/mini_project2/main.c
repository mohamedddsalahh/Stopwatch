/*
 * main.c
 *
 *  Created on: Sep 14, 2022
 *      Author: User
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"Prototypes.h"

#define DELAY 600						//Delay time between toggling the six 7-segments

unsigned short sec = 0;
unsigned short min = 0;
unsigned short hr  = 0;

ISR(INT0_vect){
	sec = 0;	//Set the seconds to zero
	min = 0;	//Set the minutes to zero
	hr = 0;		//Set the hours to zero
}
ISR(INT1_vect){				//Stop the clock by disabling CS11 and CS00 -> Pre-scaler bits
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10);
}
ISR(INT2_vect){
	TCCR1B |= (1<<CS11) | (1<<CS10);	//Start Clock by enabling CS11 and CS00
}

ISR(TIMER1_COMPA_vect){
	sec ++;				//Increment seconds by 1 every Timer1 interrupt
	if(sec > 59){
		min++;			//Increment minutes by 1 after 60 seconds
		sec = 0;		//Reset the seconds to zero
	}
	if(min>59){
		hr++;			//Increment hours by 1 after 60 seconds
		min = 0;		//Reset the minutes to zero
	}
	if(hr>23){
		sec= 0;			//Reset the seconds to zero after 24 hours
		min = 0;		//Reset the minutes to zero after 24 hours
	}
}

void main(void){
	DDRC  |= 0x0F;				//Pin 0,1,2,3 Port c -> o/p
	PORTC &= 0xF0;				//Outputs initially = 0;

	DDRA  |= 0x3F;				//Pin 0,1,2,3,4,5 Port a -> o/p
	PORTA &= 0xC0;				//All segments are disabled

	DDRD  &=~(1<<INT0);		    //Internal pull up PB -> i/p
	PORTD |= (1<<PD2); 		    //Pull up enabled

	DDRD  &=~(1<<INT1);			//Externally pulled up PB -> i/p

	DDRB  &=~(1<<INT2);			//Internal Pull up PB -> i/p
	PORTB |= (1<<PD2);			//Pull up enabled

	SREG  |= (1<<7);			//Enable I-bit

	INT0_init();
	INT1_init();
	INT2_init();
	Timer1_init();

	while(1){

		PORTA|=(1<<PA0);		//Enable first segment
		PORTC &= 0xF0;			//Clear PORTA
		PORTC |= (sec%10);		//Write seconds to first segment -> unit number

		_delay_us(DELAY);
		PORTA &= 0xC0;			//Disable all segments

		PORTA|=(1<<PA1);		//Enable second segment
		PORTC &= 0xF0;			//Clear PORTA
		PORTC |= (sec/10);		//Write seconds to second segment -> tenth number

		_delay_us(DELAY);
		PORTA &= 0xC0;			//Disable all segments

		PORTA|=(1<<PA2);		//Enable third segment
		PORTC &= 0xF0;			//Clear PORTA
		PORTC |= (min%10);		//Write minutes to third segment -> unit number

		_delay_us(DELAY);
		PORTA &= 0xC0;			//Disable all segments

		PORTA|=(1<<PA3);		//Enable fourth segment
		PORTC &= 0xF0;			//Clear PORTA
		PORTC |= (min/10);		//Write minutes to fourth segment -> tenth number

		_delay_us(DELAY);
		PORTA &= 0xC0;			//Disable all segments

		PORTA|=(1<<PA4);		//Enable fifth segment
		PORTC &= 0xF0;			//Clear PORTA
		PORTC |= (hr%10);		//Write hours to fifth segment -> unit number

		_delay_us(DELAY);
		PORTA &= 0xC0;			//Disable all segments

		PORTA|=(1<<PA5);		//Enable sixth segment
		PORTC &= 0xF0;			//Clear PORTA
		PORTC |= (hr/10);		//Write hours to sixth segment -> tenth number

		_delay_us(DELAY);
		PORTA &= 0xC0;			//Disable all segments

	}
}

