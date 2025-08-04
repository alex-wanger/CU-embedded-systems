#ifndef TWI_H 
#define TWI_H  

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum twi_status_t {
    SUCCESS,
    BUSY,
    DISABLED,
    INIT_FAILURE,
    NOMINAL,
    OVERWRITE_FAILURE
} twi_status_t;

typedef struct twi_message_t {
    uint8_t address; // 7-bit address, LSB is R/W bit 
    uint8_t *buffer;
    size_t size;
} twi_message_t;

typedef struct twi_isr_t {
    bool idle;
    twi_message_t *messages;
    size_t message_count;
    twi_status_t status;
} twi_isr_t;
// Function declarations
twi_status_t twi_init(const uint32_t scl_frequency);
twi_status_t twi_enqueue(twi_message_t *messages, size_t count);
twi_status_t twi_status(void);
twi_status_t twi_cancel(void);
twi_status_t twi_deinit(void);

#endif
