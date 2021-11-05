#include "timer.h"

typedef enum {
    STATE_INIT,
    STATE_ACTIVE,
    STATE_PASSIVE,
} main_state_t;

int cb(void *ctx) {
    return 0;
}

void main(void) {
    static main_state_t state = STATE_INIT;
    // Include examples on how the timer can be used here in the main function

    switch (state)
    {
    case STATE_INIT:
        callback_register(&cb, 1, NULL);
        /* Init stuff here */
        state = STATE_ACTIVE;
        break;
    
    case STATE_ACTIVE:
        /* Do active part here */
        state = STATE_PASSIVE;
        break;
    
    case STATE_PASSIVE:
        /* Do passive part here */
        break;
    
    default:
        /* Default handler should not be hit in this case */
        break;
    }

    return;
}