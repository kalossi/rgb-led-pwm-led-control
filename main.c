#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

void InitADC()
{
// Select Vref=AVcc
	ADMUX |= (1 << REFS1) | (1 << REFS0) | (1 << ADLAR); //| (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
//set prescaller to 128 and enable ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

uint16_t ReadADC(uint8_t ADCchannel)
{
//select ADC channel with safety mask
ADMUX = (ADMUX & 0xf0) | (ADCchannel & 0x0f);
//single conversion mode
	ADCSRA |= (1 << ADSC); //start conversion
// wait until ADC conversion is complete
	while (ADCSRA & (1 << ADSC)) {
		return ADC;
	}
}

int main()
{
	DDRB = 0xff;  // all outputs
	PORTB = 0xff; // active LOW (see circuit)
	DDRC = 0x00;
	PORTC = 0x00;
	InitADC();
	// sei();

	// CLKPR |= (1 << CLKPCE) | (0 << CLKPS3) | (0 << CLKPS2) | (1 << CLKPS1) | (1 << CLKPS0);
	// ADMUX |= (1 << REFS0) | (1 << ADLAR);
	// ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE) | (1 << ADEN);
	// ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);

	while (1) {// otherwise routine stops
		PORTB = (ReadADC(0));
	}
}

// ISR (ADC_vect)
// {
// 	adc_data = ADCH;
// 	_delay_ms(100);
// 	PORTB = adc_data;
// 	_delay_ms(100);
// }
