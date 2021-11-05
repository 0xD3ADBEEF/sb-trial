#include <stdlib.h>
#include <stdio.h>
#include "events/events.h"
#include "timer.h"
#include "state_machine.h"
#include "events.h"

typedef enum {
    STATE_INIT,
    STATE_ACTIVE,
    STATE_PASSIVE,
} main_state_t;

int cb(void *ctx) {
    return 0;
}

int on_init(void * arg) {
    printf("Initial state!\n");
}
int on_active(void * arg) {
    printf("Active state!\n");
}
int on_passive(void * arg) {
    printf("Passive state!\n");
}

int main(void) {
    Event_t evt_buf[10] = {0};
    Evt_que_t que = mk_evt_que(10, evt_buf);

    State_t states[3] = {
        {STATE_INIT, on_init},
        {STATE_ACTIVE, on_active},
        {STATE_PASSIVE, on_passive}
    };

    Trans_t trans[2] = {
        mk_trans(EVENT_NONE, &states[0], &states[1]),
        mk_trans(EVENT_NONE, &states[1], &states[1])
        // mk_trans(2, &states[1], &states[2]),
        // mk_trans(3, &states[2], &states[1]),
        // mk_trans(EVENT_NONE, &states[2], &states[2])
    };
    State_machine_t stm = mk_stm(&states[0], 4, trans, &que);


    for(size_t i = 0;i < 10; ++i) {
        execute(&stm);
    }

    
    
    // static main_state_t state = STATE_INIT;
    // // Include examples on how the timer can be used here in the main function

    // switch (state)
    // {
    // case STATE_INIT:
    //     callback_register(&cb, 1, NULL);
    //     /* Init stuff here */
    //     state = STATE_ACTIVE;
    //     break;
    
    // case STATE_ACTIVE:
    //     /* Do active part here */
    //     state = STATE_PASSIVE;
    //     break;
    
    // case STATE_PASSIVE:
    //     /* Do passive part here */
    //     break;
    
    // default:
    //     /* Default handler should not be hit in this case */
    //     break;
    // }

    return EXIT_SUCCESS;
}