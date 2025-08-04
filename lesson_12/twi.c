#include <avr/interrupt.h>
#include <avr/io.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>j
#include "twi.h"

static volatile twi_isr_t twi_isr;

static bool scl_parameters(const uint32_t scl_frequency, uint8_t *output_array)
{
    if (output_array[0] == 0 || output_array[1] == 0)
    {
        return false;
    }
    if ((((F_CPU / scl_frequency) - 16) / 2) != output_array[1] * output_array[0])
    {
        return false;
    }
    return true;
}

void calculate_clock(uint64_t SRC, uint8_t *output_array)
{
    // can do this cleaner with 1 if statement and a forloop
    uint8_t prescaler = 0;
    uint8_t twbr = 0;

    uint64_t TWBR_TIMES_SCALER = (((F_CPU / SRC) - 16) / 2);
    if (TWBR_TIMES_SCALER > 0 && TWBR_TIMES_SCALER < 255)
    {
        prescaler = 1;
        twbr = TWBR_TIMES_SCALER;
    }
    else if (TWBR_TIMES_SCALER > 255 && TWBR_TIMES_SCALER < 1023)
    {
        prescaler = 4;
        twbr = (TWBR_TIMES_SCALER / 4);
    }
    else if (TWBR_TIMES_SCALER > 1023 && TWBR_TIMES_SCALER < 4095)
    {
        prescaler = 16;
        twbr = (TWBR_TIMES_SCALER / 16);
    }
    else if (TWBR_TIMES_SCALER > 4095 && TWBR_TIMES_SCALER < 16383)
    {
        prescaler = 64;
        twbr = (TWBR_TIMES_SCALER / 64);
    }
    (output_array)[0] = prescaler; // Store the prescaler value
    (output_array)[1] = twbr;      // Store the TWBR value
}

twi_status_t twi_init(const uint32_t scl_frequency)
{
    // initialize pull-up resistors
    DDRD = DDRD & 0b11001111;   // set as pins 5 and 4 as low
    PORTD = PORTD | 0b11001111; // set the associated port to high

    uint8_t output[2];
    calculate_clock(scl_frequency, (uint8_t *)&output);
    if (scl_parameters(output[1], (uint8_t *)&output))
    {
        // Set the prescaler and TWBR values
        TWSR = output[1]; // Set prescaler
        TWBR = output[0]; // Set TWBR value
    }
    else
    {
        return INIT_FAILURE; // SOMETHING BAD IS HAPPENING
    };

    twi_isr.idle = true;
    twi_isr.status = NOMINAL;

    TWCR = (1 << TWEN) | (1 << TWIE);

    if (!twi_isr.idle)
    {
        return BUSY;
    }
    twi_isr.idle = true;

    return SUCCESS;
}

twi_status_t twi_enqueue(twi_message_t *messages, size_t count)
{
    uint8_t sreg = SREG;

    cli();

    if (!(TWCR & (1 << TWEN)))
    {
        return DISABLED;
    }

    if (TWCR & (1 << TWIE))
    {
        return BUSY;
    }

    twi_isr = (twi_isr_t){
        .idle = false,
        .messages = messages,
        .message_count = count,
    };

    SREG = sreg;

    TWCR = TWCR | (1 << TWSTA); // start the call !

    return SUCCESS;
}

int main(void)
{
    // This is a simple C program that does nothing
    twi_init(100000);
    return 0;
}

// TWSR values
// Mnemonics:
// TW_MT_xxx - master transmitter
// TW_MR_xxx - master receiver
// TW_ST_xxx - slave transmitter
// TW_SR_xxx - slave receiver
ISR(TWI_vect)
{

    if ((TWCR & (1 << TWWC)))
    {
        twi_isr.status = OVERWRITE_FAILURE; // if we are in the TWWC state, we have a problem
        twi_isr.idle = true;                // we are idle now
        return;
    }

    twi_message_t *message = twi_isr.messages;

    switch (TW_STATUS)
    {
    case TW_START:
    case TW_REP_START:
        TWDR = message->address;

        twi_isr.message_count--;
        twi_isr.messages++;

        uint8_t temp = TWCR & ~(1 << TWSTA); // we just got the start state, we need to clear it

        bool read_write = message->address & 1; // if 1, we are writing ! if 0 we are reading

        if (read_write)
        {
            temp |= 1 << TWEA;
        }
        // if we are writing, we need to clear the TWEA bit
        TWCR = temp;

        return;
    case TW_MT_SLA_ACK: // SLA+W transmitted, ACK

    case TW_MT_DATA_ACK:
        if (message->size > 0)
        {                            // no more bytes to send in the current message
            --twi_isr.message_count; // we can subtract one from the number of messages left
            uint8_t temp;
            if (twi_isr.message_count != 0)
            {
                ++twi_isr.messages; // move to next message
                temp = TWSTA;       // repeated START
            }
            else
            {
                temp = TWSTO; // STOP
            }

            if (temp == TWSTO)
            {
                twi_isr.status = NOMINAL;
            }

            TWCR |= 1 << temp;

            return;
        }

        TWDR = *(message->buffer++);

        TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);

        return;

    case TW_MT_DATA_NACK: // Data byte transmitted, NACK received
        *(message->buffer++) = TWDR;
        TWCR = (1 << TWEN) |                               // TWI Interface enabled
               (0 << TWIE) | (1 << TWINT) |                // Disable TWI Interrupt and clear the flag
               (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | // Initiate a STOP condition.
               (0 << TWWC);                                //
        break;

    case TW_MT_ARB_LOST: // Arbitration lost in SLA+R/W or data bytes
        // Handle arbitration lost
        break;
    case TW_MR_SLA_ACK: // SLA+R transmitted, ACK received
        // Handle SLA+R ACK
        break;
    case TW_MR_SLA_NACK: // SLA+R transmitted, NACK received
        // Handle SLA+R NACK
        break;
    case TW_MR_DATA_ACK: // Data byte received, ACK sent
        // Handle data byte ACK
        break;
    case TW_MR_DATA_NACK: // Data byte received, NACK sent
        // Handle data byte NACK
        break;
    case TW_SR_SLA_ACK: // Own SLA+W received, ACK returned
        // Handle own SLA+W ACK
        break;
    case TW_SR_GCALL_ACK: // General call received, ACK returned
        // Handle general call ACK
        break;
    case TW_SR_DATA_ACK: // Data byte received, ACK returned
        // Handle data byte received with ACK returned
        break;
    case TW_SR_GCALL_DATA_ACK: // General call data byte received, ACK returned
        // Handle general call data byte with ACK returned
        break;
    case TW_SR_STOP: // Stop or repeated start condition received while addressed as slave
        // Handle stop or repeated start condition in slave mode
        break;
    default:
        TWCR = (1 << TWEN) |                               // Enable TWI-interface and release TWI pins
               (0 << TWIE) | (0 << TWINT) |                // Disable Interrupt
               (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | // No Signal requests
               (0 << TWWC);
    }
}