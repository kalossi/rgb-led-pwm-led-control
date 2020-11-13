#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

void initADC()
{
// Select Vref=AVcc and left adjust ADC register
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR);
//set prescaller to 128 and enable ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

void initPWM()
{
// Fast PWM: WGM22, WGM21 and WGM20 = 111, clear OC2A on compare match
	TCCR2A |= (1 << COM2A1) | (1 << COM2A0) | (0 << WGM21) | (1 << WGM20) | (1 << COM2A1) | (0 << COM2A0);
	TCCR2B |= (1 << WGM22);
// No prescaler: CS22, CS21 and CS20 = 001
	TCCR2B |= (1 << CS20);
	TCNT2 |= 0x00;
}

uint8_t readADCH(uint8_t ADCchannel)
{
//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xf0) | (ADCchannel & 0x0f);
//single conversion mode
	ADCSRA |= (1 << ADSC); //start conversion
// wait until ADC conversion is complete
	while (!(ADCSRA & (1 << ADIF))) {
	}
// clear flag
	ADCSRA |= (1 << ADIF);
// return the 8 msb of ADC register
	return ADCH;
}

int main()
{
	DDRB = 0xff;  // all outputs
	PORTB = 0x00;
	DDRC = 0x00; // all inputs
	PORTC = 0xff;
	initADC();
	initPWM();
// otherwise routine stops
	while (1) {
		OCR2A |= (readADCH(0));
	}
}