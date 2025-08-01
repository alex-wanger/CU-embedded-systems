#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000UL
#define TIMER0_PRESCALER 8

volatile uint8_t counter = 0;
volatile uint64_t microseconds = 0;
volatile uint64_t milliseconds = 0;
volatile uint64_t seconds = 0;

ISR (TIMER0_OVF_vect) {
   microseconds += 128;
    if (microseconds >= 1000) {
        milliseconds += 1;
        microseconds -= 1000;
    }
}

int main(void) {
    DDRB |= _BV(DDB5); // Set PB5 as output 
    
    TCNT0 = 0; 
    
    TCCR0B = (1 << CS01);  

    TCCR0A = 0; // Normal mode

    TIMSK0 = (1 << TOIE0); // Enable Timer0 overflow interrupt

    sei();
    while (1) 
    {
       ;; 
    } 
}