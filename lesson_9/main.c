#include <avr/io.h>
#include <inttypes.h>
const int STORAGE = 5;
const int SHIFT_PIN = 6;
const int RESET_PIN = 7;
const int INPUT_PIN = 4;

// static const uint8_t ASCII = {0b00000011};

void initalize() {
    for (int i = 2; i < 6; i++) {
        DDRB |= (1 << i);
    } 
    //set up the first 4 DIGIT pins to be outputs
    for (int i = 4; i < 8; i++) {
        DDRD |= (1 << i);
    }
    //set up the 3 shift register pins to be outputs
    // PORTD |= ~(1 << RESET_PIN);
    //set the RESET pin to be LOW -> Flush the memory map
    // PORTD = ~PORTD;
    PORTD = 0b10000000;
}

void loadDigit(uint8_t digit) {
    PORTD |= (1 << SHIFT_PIN); 
    //SET SHIFT PIN TO HIGH
    
    if (digit == 1) {
        PORTD |= (1 << INPUT_PIN);
    }
    else {
        PORTD |= (0 << INPUT_PIN);
    }

    PORTD &= ~(1 << SHIFT_PIN);
}


void loadToStorage() {
    PORTD |= (1 << STORAGE);
    PORTD &= ~(1 << STORAGE);
    //write HIGH to STORAGE making it send to output;
}

int main(void) { 
    initalize();
    loadDigit(1);
    loadToStorage();
}



