#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <stdio.h>
// #include <math.h>

void initADC()
{
// Select Vref=AVcc and left adjust ADC register
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR);
//set prescaller to 8, enable ADC, ADC interrupts and auto trigger
	ADCSRA |= (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN) | (1 << ADATE) | (1 << ADIE);
}

void initPWM()
{
// PWM phase correct: WGM22, WGM21 and WGM20 = 101, clear OC2A on compare match
	TCCR2A |= (1 << COM2A1) | (1 << COM2A0) | (0 << WGM21) | (1 << WGM20) | (1 << COM2A1) | (0 << COM2A0);
	TCCR2B |= (1 << WGM22);
// No prescaler: CS22, CS21 and CS20 = 001
	TCCR2B |= (1 << CS20);
// enable interrupt on timer/counter 2A
	TIMSK2 |= (1 << OCIE2A);
// initial compare value
	OCR2A = 5;
}

uint8_t readADCH(uint8_t ADCchannel)
{
//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xf0) | (ADCchannel & 0x0f);
//single conversion mode, ADC prescaler 8
	ADCSRA |= (1 << ADSC) | (1 << ADPS1);  //start conversion
// wait until ADC conversion is complete
	while (!(ADCSRA & (1 << ADIF))) {
	}
// clear flag
	ADCSRA |= (1 << ADIF);
// return the 8 msb of ADC register
	return ADCH - 1;
}

int main()
{
	CLKPR |= (1 << CLKPCE) | (1 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
	DDRB = 0xff; // all outputs
	PORTB = 0x00;
	DDRD = 0xff;  // all outputs
	PORTD = 0xff; // active low leds
	DDRC = 0x01; // PINC0 for a led
	PORTC = 0x01;
	initADC();
	initPWM();
	sei();
// otherwise routine stops
	while (1) {
		// PORTD = ~(readADCH(0));
	}
}

ISR (TIMER2_COMPA_vect)
{
// change the timer compare register 2A to match the 8 bits of ADCH register
	OCR2A = ~(readADCH(0));
}

// ISR (ADC_vect)
// {
// 	OCR2A = (readADCH(0));
// }
