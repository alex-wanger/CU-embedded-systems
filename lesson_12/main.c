#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "twi.c"
#include "twi.h"

int main(void)
{
    sei();
    twi_status_t status = twi_init(100000);

    while (status != SUCCESS)
        ;

    twi_message_t messages[2] = {
        {
            .address = TWI_WRITE_ADDRESS(0x68),
            .buffer = (uint8_t[]){0x75},
            .size = 1,
        },
        {
            .address = TWI_READ_ADDRESS(0x68),
            .buffer = (uint8_t[]){},
            .size = 1,
        }};

    twi_enqueue(messages, 2);

    while (twi_status() != SUCCESS)
        ;
    while (1)
        ;
    return 0;
}
