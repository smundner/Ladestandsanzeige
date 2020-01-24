#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>

#define LOW_BAT 5/1024*2.8
#define MID_BAT 5/1024*3
#define FULL_BAT 5/1024*3.7

ISR(WDT_vect){
	WDTCR|=(1<<WDIE)|(1<<WDE);
}

uint16_t readADC(){
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	uint16_t result = ADC;
	return result;
}

int main(){
	//config output	
	DDRB|=(1<<PB0)|(1<<PB1)|(1<<PB2);
	PORTB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2));
	//config ADC
	ADMUX |=(1<<MUX0)|(1<<MUX1);
	ADCSRA |= (1<<ADEN)|(1<<ADPS2);
	DIDR0|=(1<<ADC3D);
	//config sleepmode
	MCUCR|=(1<<SM1);
	//config the dog
	WDTCR|=(1<<WDIE)|(1<<WDE)|(1<<WDP3)|(1<<WDP0);
	sei();
	while(1){
		uint16_t batVoltageRAW=readADC();
		if(LOW_BAT>batVoltageRAW){
			PORTB&=(1<<PB0)|(1<<PB1)|(1<<PB2);
			PORTB|=(1<<PB0);
		}else if(batVoltageRAW<MID_BAT){
			PORTB&=(1<<PB0)|(1<<PB1)|(1<<PB2);
			PORTB|=(1<<PB1);		
		}else if(MID_BAT<batVoltageRAW){
			PORTB&=(1<<PB0)|(1<<PB1)|(1<<PB2);
			PORTB|=(1<<PB2);		
		}
		sleep_cpu();
	}

	return 0;
}
