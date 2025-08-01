#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    SUCCESS,
    BUSY,
    INIT_FAILURE,
    NOMINAL
} twi_status_t;

typedef struct twi_message_t {
    unsigned char *data;
    unsigned char *dataSize;
} twi_message_t;

static struct {

}
// Function declarations
twi_status_t twi_init(const uint8_t scl_frequency);
twi_status_t twi_enqueue(twi_message_t *messages, size_t count);
twi_status_t twi_status(void);
twi_status_t twi_cancel(void);
twi_status_t twi_deinit(void);

#endif
