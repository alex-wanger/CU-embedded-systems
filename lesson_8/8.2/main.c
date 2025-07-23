#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << 5); 

    //00000000 & NOT (00000100) BECOMES
    //11111011

    DDRD &= ~(1 << 2);

    while (1) {
        if ((PIND & (1 << PIND2))) {
            PORTB ^= (1 << PORTB5);
            _delay_ms(300);  
        }
    }

    return 0;
}
