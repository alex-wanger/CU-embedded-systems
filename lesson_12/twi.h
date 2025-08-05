#ifndef TWI_H
#define TWI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <avr/io.h>

// TWI Status Register constants (these might be defined elsewhere but including for completeness)
#ifndef TW_STATUS
#define TW_STATUS (TWSR & 0xF8)
#endif

typedef enum twi_status_t
{
    SUCCESS,
    BUSY,
    DISABLED,
    INIT_FAILURE,
    NOMINAL,
    OVERWRITE_FAILURE,
    NACK_FAILURE,
    BUS_ERROR,
    TIMEOUT,
    INVALID_PARAMETER
} twi_status_t;

typedef struct twi_message_t
{
    uint8_t address; // 7-bit address with R/W bit in LSB (bit 0: 0=write, 1=read)
    uint8_t *buffer; // Pointer to data buffer
    size_t size;     // Number of bytes to transfer
} twi_message_t;

typedef struct twi_isr_t
{
    volatile bool idle;
    volatile twi_message_t *messages;
    volatile size_t message_count;
    volatile twi_status_t status;
} twi_isr_t;

// Function declarations
twi_status_t twi_init(const uint32_t scl_frequency);
twi_status_t twi_enqueue(twi_message_t *messages, size_t count);
twi_status_t twi_get_status(void); // Renamed from twi_status to avoid naming conflict
twi_status_t twi_cancel(void);
twi_status_t twi_deinit(void);

#define TWI_WRITE_ADDRESS(addr) ((addr) << 1)      // Shift 7-bit address left, R/W bit = 0
#define TWI_READ_ADDRESS(addr) (((addr) << 1) | 1) // Shift 7-bit address left, R/W bit = 1

#endif /* TWI_H */