/*
 * Functions.c
 *
 *  Created on: Sep 14, 2022
 *      Author: User
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


void INT0_init(void){
	MCUCR |= (1<<ISC01);				//Interrupt with falling edge
	GICR |= (1<<INT0);					//Enable INT0
}
void INT1_init(void){
	MCUCR |= (1<<ISC11) | (1<<ISC10);	//Interrupt with rising edge
	GICR |= (1<<INT1);					//Enable INT1
}
void INT2_init(void){
	MCUCSR &= ~(1<<ISC2);				//Interrupt with falling edge
	GICR |= (1<<INT2);					//Enable INT2
}

void Timer1_init(void){
	TCCR1A |= (1<<FOC1A);	  						 	//Force output compare for timer/control register A
	TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);	 	//64 pre-scaler
	TCNT1 = 0;											//Initial counting value
	OCR1A = 15625;									  	//15625 counts for 1 interrupt every 1 second
	TIMSK |= (1<<OCIE1A);								//Enable Timer1 A interrupt
}


