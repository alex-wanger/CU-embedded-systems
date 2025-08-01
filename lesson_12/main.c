#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h> 
#include "main.h"


twi_status_t twi_init(const uint8_t scl_frequency);
{
    //initialize pull-up resistors
    DDRD = DDRD & 11001111; // set as pins 5 and 4 as low
    PORTD = PORTD | 11001111; // set the associated port to high 
    
    uint8_t output[2];
    calculate_clock(target_scl, &output);
    if (scl_parameters(target_scl, &output)) {
        // Set the prescaler and TWBR values
        TWSR = output[0]; // Set prescaler
        TWBR = output[1]; // Set TWBR value
    } else {
        return INIT_FAILURE; // SOMETHING BAD IS HAPPENING
    }

    return SUCCESS;
}

twi_status_t twi_enqueue(twi_message_t *messages, size_t count) {
    if (TWCR & (1 << TWIE)) {
        return BUSY;
    }


    return SUCCESS;
}

void calculate_clock(uint64_t SRC, uint8_t *output_array) {
    //can do this cleaner with 1 if statement and a forloop
    uint8_t prescaler = 0;
    uint8_t twbr = 0;
    
    uint64_t TWBR_TIMES_SCALER = (((F_CPU / SRC) - 16) / 2 ); 
    if (TWBR_TIMES_SCALER > 0 && TWBR_TIMES_SCALER < 255) {
        prescaler = 1;
        twbr = TWBR_TIMES_SCALER;
    }
    else if (TWBR_TIMES_SCALER > 255 && TWBR_TIMES_SCALER < 1023) {
        prescaler = 4;
        twbr = (TWBR_TIMES_SCALER / 4);
    }
    else if (TWBR_TIMES_SCALER > 1023 && TWBR_TIMES_SCALER < 4095) {
        prescaler = 16;
        twbr = (TWBR_TIMES_SCALER / 16);
    }
    else if (TWBR_TIMES_SCALER > 4095 && TWBR_TIMES_SCALER < 16383) {
        prescaler = 64;
        twbr = (TWBR_TIMES_SCALER / 64);
    }
    (output_array)[0] = prescaler; // Store the prescaler value
    (output_array)[1] = twbr; // Store the TWBR value
}

static bool scl_parameters(const uint32_t scl_frequency, uint8_t *output_array) {
    if (output_array[0] == 0 || output_array[1] == 0) {
        return false;
    }
    if ((((F_CPU / scl_frequency) - 16) / 2 ) != output_array[1] * output_array[0]) {
        return false;
    }
    return true;
}

int main(void) {
    // This is a simple C program that does nothing
    initalization();
    return 0;
}

// TWSR values
// Mnemonics:
// TW_MT_xxx - master transmitter
// TW_MR_xxx - master receiver
// TW_ST_xxx - slave transmitter
// TW_SR_xxx - slave receiver
// ISR(TWI_vect) {
    
//     switch (TW_STATUS)
//     {
//     case TW_MR_DATA_ACK:
//         /* code */
//         break;
//     case TW_MR_DATA_NACK:
//         /* code */
//         break;
//     case TW_MR_SLA_ACK:
//     //
//         break;
//     case TW_MR_SLA_NACK:
//     //
//         break;  
//     case TW_MR_DATA_ACK:
//     //
//         break;
//     case TW_MR_DATA_NACK:
//     //
//     default:
//         break;
//     }
// }
