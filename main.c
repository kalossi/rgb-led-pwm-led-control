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

uint16_t readADC(uint8_t ADCchannel)
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
	PORTB = 0xff; // active LOW (see circuit)
	initADC();
// otherwise routine stops
	while (1) {
// only for testing purposes - leds active low
		PORTB = ~(readADC(0));
	}
}