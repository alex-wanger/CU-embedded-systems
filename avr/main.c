#ifndef F_CPU
#define F_CPU 16000000UL  // Define CPU frequency for delay functions (e.g., 16MHz)
#endif

#include <avr/io.h>    // Includes I/O port definitions
#include <util/delay.h>  // Includes delay functions like _delay_ms()

int main(void) {
    __asm__("break"::);
    return 0;
}

