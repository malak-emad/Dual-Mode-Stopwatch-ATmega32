/*
 * project.c
 *
 *  Created on: Sep 12, 2024
 *      Author: Malak
 */
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

unsigned char digit1=0, digit2=0, digit3=0, digit4=0, digit5=0, digit6=0;
unsigned char pause_flag = 0;					//To activate Pause
unsigned char countUp_flag = 0;					//To increment in the CountUp mode
unsigned char countDown_mode_flag = 0;			//To switch to the CountDown mode
unsigned char countDown_flag = 0;				//To decrement in the CountDowm mode

/*
 *Digit1 and digit2 represents the Hours digits of the 7 segment
 *Digit3 and Digit4 represents the Minutes digits of the 7 segment
 *Digit5 and digit6 represents the seconds digits of the 7 segment
 */

void showNumber(unsigned char digit1, unsigned char digit2, unsigned char digit3, unsigned char digit4, unsigned char digit5, unsigned char digit6){
	PORTA = 0X01;
	PORTC = ((PORTC & 0XF0)|(digit1 & 0X0F));
	_delay_ms(2);

	PORTA = 0X02;
	PORTC = ((PORTC & 0XF0)|(digit2 & 0X0F));
	_delay_ms(2);

	PORTA = 0X04;
	PORTC = ((PORTC & 0XF0)|(digit3 & 0X0F));
	_delay_ms(2);

	PORTA = 0X08;
	PORTC = ((PORTC & 0XF0)|(digit4 & 0X0F));
	_delay_ms(2);

	PORTA = 0X10;
	PORTC = ((PORTC & 0XF0)|(digit5 & 0X0F));
	_delay_ms(2);

	PORTA = 0X20;
	PORTC = ((PORTC & 0XF0)|(digit6 & 0X0F));
	_delay_ms(2);
}

void Timer1_Init_CTC_mode(void){
	TCNT1 = 0;
	OCR1A = 15610;

	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);		//Prescaler 1024

	TIMSK |= (1<<OCIE1A);
}

void INT0_Init(void){
	DDRD &= ~(1<<2);				//Input Pin PD2 -- Reset (Push Button)
	PORTD |= (1<<2);				//Internal Pull Up Resistor
	MCUCR |= (1<<ISC01);			//Falling Edge
	GICR |= (1<<INT0);
}

void INT1_Init(void){
	DDRD &= ~(1<<3);					//Input Pin PD3 -- Pause (Push Button)
	MCUCR |= (1<<ISC11) | (1<<ISC10);	//Raising Edge
	GICR |= (1<<INT1);
}

void INT2_Init(void){
	DDRB &= ~(1<<2);				//Input Pin PB2 -- Resume (Push Button)
	PORTB |= (1<<2);				//Internal Pull Up Resistor
	MCUCSR &= ~(1<<ISC2);			//Falling Edge
	GICR |= (1<<INT2);
}


int main(void){
	DDRA = 0X3F;					//Control the display of the 7 segment
	DDRC = 0X0F;					//Control the number shown on the 7 segment

	DDRB &= ~(1<<7);				//Input Pin PB7 --  Toggles between Increment and Countdown modes
	PORTB |= (1<<7);				//Internal Pull Up Resistor

	DDRB &= ~(0X7B);				//Input Pins PB0, PB1, PB3, PB4, PB5, PB6 -- Clock Adjustment
	PORTB |= 0X7B;					//Internal Pull Up Resistors

	DDRD |= (1<<0);					//Output Pin PD0 -- Buzzer
	DDRD |= (1<<4) | (1<<5);		//Output Pins PD4, PD5 -- Counting Up/Down LEDs
	PORTD |= (1<<4);				//Red Led Initially On
	PORTD &= ~(1<<0) & ~(1<<5);		//Yellow Led and Buzzer Initially Off

	SREG |= (1<<7);

	Timer1_Init_CTC_mode();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	showNumber(digit1, digit2, digit3, digit4, digit5, digit6);		//initial value of 7 segment is 000000


	while(1){
		showNumber(digit1, digit2, digit3, digit4, digit5, digit6);

		if(!(PINB & (1<<7))){				//Toggle the mode
			_delay_ms(30);
				if(!(PINB & (1<<7))){
				countDown_mode_flag ^= (1<<0);

				while(!(PINB & (1<<7))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}
		}

		if(countUp_flag){					//CountUp mode
			countUp_flag = 0;

			PORTD &= ~(1<<0);				//Buzzer off

			PORTD |= (1<<4);				//Red Led on
			PORTD &= ~(1<<5);				//Yellow Led off

			if(!pause_flag){				//Start CountUp
					digit6++;

					if(digit6 == 10){
						digit6 = 0;
						digit5++;
					}

					if(digit5 == 6){
						digit5 = 0;
						digit4++;
					}

					if(digit4 == 10){
						digit4 = 0;
						digit3 ++;
					}

					if(digit3 == 6){
						digit3 = 0;
						digit2++;
					}

					if(digit2 == 10){
						digit2 = 0;
						digit1++;
					}

					if(digit1 == 10){
						digit1 = 0;
					}
			}
		}

		//CountDown mode
		else if(countDown_mode_flag && pause_flag){	//Clock Adjustment

			PORTD &= ~(1<<0);					//Buzzer off

			PORTD &= ~(1<<4);					//Red Led Off
			PORTD |= (1<<5);					//Yellow Led On

			if(!(PINB & (1<<0))){				//Decrement hours
				_delay_ms(5);
				if(!(PINB & (1<<0))){
					if((digit2 == 0) && (digit1 == 0)){
						digit2 = 0;
						digit1 = 0;
					}
					else if((digit2 == 0)){
						digit2 = 9;
						digit1--;
					}
					else digit2--;
				}
				while(!(PINB & (1<<0))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}

			if(!(PINB & (1<<1))){				//Increment hours
				_delay_ms(5);
				if(!(PINB & (1<<1))){
					if((digit2 == 3) && (digit1 == 2)){
						digit2 = 3;
						digit1 = 2;
					}
					else if((digit2 == 9)){
						digit2 = 0;
						digit1++;
					}
					else digit2++;
				}
				while(!(PINB & (1<<1))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}

			if(!(PINB & (1<<3))){				//Decrement minutes
				_delay_ms(5);
				if(!(PINB & (1<<3))){
					if((digit4 == 0) && (digit3 == 0)){
						digit3 = 0;
						digit4 = 0;
					}
					else if((digit4 == 0)){
						digit4 = 9;
						digit3--;
					}
					else digit4--;
				}
				while(!(PINB & (1<<3))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}

			if(!(PINB & (1<<4))){				//Increment minutes
				_delay_ms(5);
				if(!(PINB & (1<<4))){
					if((digit4 == 9) && (digit3 == 5)){
						digit4 = 9;
						digit3 = 5;
					}
					else if((digit4 == 9)){
						digit4 = 0;
						digit3++;
					}
					else digit4++;
				}
				while(!(PINB & (1<<4))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}

			if(!(PINB & (1<<5))){				//Decrement seconds
				_delay_ms(5);
				if(!(PINB & (1<<5))){
					if((digit6 == 0) && (digit5 == 0)){
						digit6 = 0;
						digit5 = 0;
					}
					else if((digit6 == 0)){
						digit6 = 9;
						digit5--;
					}
					else digit6--;

				}
				while(!(PINB & (1<<5))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}

			if(!(PINB & (1<<6))){				//Increment seconds
				_delay_ms(5);
				if(!(PINB & (1<<6))){
					if((digit6 == 9) && (digit5 == 5)){
						digit6 = 9;
						digit5 = 5;
					}
					else if((digit6 == 9)){
						digit6 = 0;
						digit5++;
					}
					else digit6++;
				}
				while(!(PINB & (1<<6))){
					showNumber(digit1, digit2, digit3, digit4, digit5, digit6);
				}
			}
		}

		else if(countDown_flag){			//Start CountDown
			countDown_flag = 0;

			PORTD &= ~(1<<4);					//Red Led Off
			PORTD |= (1<<5);					//Yellow Led On

			if(digit6 == 0){
				if(digit5 != 0){
					digit6 = 9;
					digit5--;
				}
				else if(digit4 != 0){
					digit6 = 9;
					digit5 = 5;
					digit4--;
				}
				else if(digit3 != 0){
					digit6 = 9;
					digit5 = 5;
					digit4 = 9;
					digit3--;
				}
				else if(digit2 != 0){
					digit6 = 9;
					digit5 = 5;
					digit4 = 9;
					digit3 = 5;
					digit2--;
				}
				else if(digit1 != 0){
					digit6 = 9;
					digit5 = 5;
					digit4 = 9;
					digit3 = 5;
					digit2 = 9;
					digit1--;
				}
				else{					//CountDown finished (Timer became 0)
					digit1 = digit2 = digit3 = digit4 = digit5 = digit6 = 0;

					PORTD |= (1<<0);	//Activate Buzzer
				}
			}
			else digit6--;
		}
	}
}

ISR(TIMER1_COMPA_vect){
	if(!countDown_mode_flag){			//CountUp mode
		countUp_flag = 1;
	}

	else if(!pause_flag){				//CountDown mode
		countDown_flag = 1;
		}
	}

ISR(INT0_vect){							//Reset Interrupt
	digit1 = digit2 = digit3 = digit4 = digit5 = digit6 = 0;
}

ISR(INT1_vect){							//Pause Interrupt
	pause_flag = 1;
}

ISR(INT2_vect){							//Resume Interrupt
	pause_flag = 0;
}
