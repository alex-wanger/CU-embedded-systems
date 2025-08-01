#include <stddef.h> // allow me to use null
#include "digital.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

enum {
    PORT_D,
    PORT_C,
    PORT_B
};

uint8_t *INDEX_TO_PORT[3] = {
    [PORT_D] = (uint8_t *)0x2B, 
    [PORT_C] = (uint8_t *)0x28,  
    [PORT_B] = (uint8_t *)0x25
};
// Turning a pin into the base register
uint8_t *digital_pin_to_port(uint8_t pin)
{
    uint8_t *ptr = NULL;

    if (pin < 8)
    {
        ptr = INDEX_TO_PORT[PORTD]; // PORTD
    }

    if (pin < 14 && pin >= 8)
    {
        ptr = INDEX_TO_PORT[PORT_C]; // PORT_C
    }

    if (pin < 20 && pin >= 14)
    {
        ptr = INDEX_TO_PORT[PORT_C]; // PORTD
    }

    return (uint8_t*) ptr;
}

typedef enum digital_register_type_t
{
    PORT,
    DDR,
    PIN
} digital_register_type_t;

typedef enum digital_mode_t {
    INPUT,
    OUTPUT,
    INPUT_PULLUP
}   digital_mode_t;

uint8_t *digital_port_offset(uint8_t *const port, digital_register_type_t type)
{
    return port - type;
}

uint8_t digital_pin_to_bit(uint8_t pin) {
    if (pin < 14) {
        return pin;
    }
    if (pin < 20) {
        return pin - 14;
    }
}

bool digital_pin_mode(uint8_t pin, digital_mode_t mode) {
    uint8_t* pointer = digital_pin_to_port(pin);
    pointer = digital_port_offset(pointer, DDR);

    uint8_t bit = digital_pin_to_bit(pin);
    if (mode == INPUT) {
        *pointer &= ~(1 << bit); //if the bit is already 0, will remain zero, otherwise will become zero
    } else if (mode == OUTPUT) {
        *pointer |= (1 << bit);  //if the bit is already 1, will remain one, otherwise will become one
    } else if (mode == INPUT_PULLUP) {
        *pointer &= ~(1 << bit); //make it zero
        *(digital_port_offset(pointer, PORT)) |= (1 << bit); 
    } 
        
    return 0;
}

bool digital_write(uint8_t pin, bool high) {
    uint8_t* pointer = digital_pin_to_port(pin);
    pointer = digital_port_offset(pointer, PORT);

    uint8_t bit = digital_pin_to_bit(pin);

    if (high) {
        *pointer |= (1 << bit); 
    }
    else {
        *pointer &= ~(1 << bit);
    }
    
    if (pointer == 0) {
        return 1;
        //POINTER IS NULL, PROBLEM!
    }

    return 0;
}

bool digital_read(uint8_t pin, bool * const pin_state) { 
    if (digital_pin_to_port(pin) != 0) {
        z
        uint8_t * pointer = digital_pin_to_port(pin);
        pointer = digital_port_offset(pointer, PIN);

        uint8_t bit = digital_pin_to_bit(pin);

        if (*pointer & 1 << bit) {
            *pin_state = true;
        }
        else {
            *pin_state = false;
        }
        // return false;
    }                        
    //return succesfully
    return 1;
}

int main(void) {
    bool boolean;

    digital_pin_mode(2, INPUT);
    digital_pin_mode(3, OUTPUT);
    digital_read(2, &boolean);
    // while (1) {
    // digital_read(2, &boolean);
    // digital_write(3, boolean);
    // }
   
}

