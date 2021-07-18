/*

 *
 *  Created on: MAY 10, 2021
 *      Author: AZZA OMARA
 *      Diploma 54
 */

//IMPORT LIBRARY
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//definition of the counters.
unsigned char SECOND_SEG = 0;
unsigned char MINUTE_SEG = 0;
unsigned char HOUR_SEG = 0;

//First FUN of TIMER1 of INT.
void TIMER1_Init(void)
{

	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);
	TCNT1 = 0;
	OCR1A = 15625;
	SREG|=(1<<7);
	TIMSK|=(1<<OCIE1A);
}
//FUN ISR OF INT TO TIMER 1
ISR(TIMER1_COMPA_vect)
{
	SECOND_SEG ++;
	if (SECOND_SEG == 60){
		SECOND_SEG = 0;
		MINUTE_SEG ++;
	}
	if (MINUTE_SEG == 60){
		SECOND_SEG = 0;
		MINUTE_SEG = 0;
		HOUR_SEG ++;
	}
	if (HOUR_SEG == 12){
		SECOND_SEG = 0;
		MINUTE_SEG = 0;
		HOUR_SEG = 0 ;
	}

}
//SECOUND FUN TO INT0 FOR BUTTON 2(RESET).
void INT0_Init_Reset(void)
{
	DDRD &=~(1<<PD2);
	PORTD|=(1<<PD2);
	MCUCR|=(1<<ISC01);
	GICR |=(1<<INT0);
	SREG |=(1<<7);
}
//FUN ISR FOR INT0(Falling edge using the internal pull up)
ISR(INT0_vect)
{
	SECOND_SEG = 0;
	MINUTE_SEG= 0;
	HOUR_SEG = 0 ;
}
//THERD FUN TO INT1 FOR BUTTON 3(Pause).
void INT1_Init_Pause(void)
{
	DDRD &= ~(1<<PD3);
	MCUCR|=(1<<ISC10)|(1<<ISC11);
	GICR |=(1<<INT1);
	SREG|=(1<<7);
}
//FUN ISR FOR INT1(raising edge using the external pull down).
ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS10)&~(1<<CS11)&~(1<<CS12);
}
//THERD FUN TO INT2 FOR BUTTON 1(Resume).
void INT2_Init_Resume(void)
{
	DDRB &=~(1<<PB2);
	PORTB |= (1<<PB2);
	MCUCR&=~(1<<ISC2);
	GICR |=(1<<INT2);
	SREG|=(1<<7);
}
//FUN ISR FOR INT2(Falling edge using the internal pull up).
ISR(INT2_vect)
{
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);
}

int main()
{
	DDRA = 0xff;
	PORTA = 0xff;
	DDRC = 0x0f;
	PORTC = 0x00;

//CALL FUNS.
	INT0_Init_Reset();
	INT1_Init_Pause();
	INT2_Init_Resume();
	TIMER1_Init( );

	while(1)
	{
		PORTA = (1<<5);
		PORTC = SECOND_SEG % 10;
		_delay_ms(3);
		PORTA = (1<<4);
		PORTC = SECOND_SEG / 10;
		_delay_ms(3);
		PORTA = (1<<3);
		PORTC = MINUTE_SEG % 10;
		_delay_ms(3);
		PORTA = (1<<2);
		PORTC = MINUTE_SEG / 10;
		_delay_ms(3);
		PORTA = (1<<1);
		PORTC = HOUR_SEG % 10;
		_delay_ms(3);
		PORTA = (1<<0);
		PORTC = HOUR_SEG / 10;
		_delay_ms(3);
	}

}
