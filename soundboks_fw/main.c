#include <stdlib.h>
#include <stdio.h>
#include "events/events.h"
#include "timer/timer.h"
#include "state_machine/state_machine.h"
// #include <time.h>

typedef enum {
    STATE_INIT,
    STATE_ON,
    STATE_OFF
} main_state_t;

#define EVENT_TMR 0x1UL

int tmr_evnt1(void * arg) {
    Evt_que_tp que = (Evt_que_tp)arg;
    return put_evt(que, EVENT_TMR);
}

int on_init(void * arg) {
    printf("Initial state!\n");
    return 0;
}
int on_active(void * arg) {
    printf("Active state!\n");
    return 0;
}
int on_passive(void * arg) {
    printf("Passive state!\n");
    return 0;
}

int main(void) {
    Event_t evt_buf[10] = {0};
    Evt_que_t que = mk_evt_que(10, evt_buf);
    Timer_t * tmr;

    State_t states[3] = {
        mk_state(STATE_INIT, on_init, NULL),
        mk_state(STATE_ON, on_active, NULL),
        mk_state(STATE_OFF, on_passive, NULL)
    };

    Trans_t trans[3] = {
        mk_trans(EVENT_NONE, &states[0], &states[1]),
        mk_trans(EVENT_TMR, &states[1], &states[2]),
        mk_trans(EVENT_TMR, &states[2], &states[1])
    };
    State_machine_t stm = mk_stm(&states[0], 3, trans, &que);

    tmr = timer_start(TIME_SEC(1.0), true, tmr_evnt1, &que);
    while(is_running(tmr)) {
        execute(&stm);
        update_timers();
    }

    return EXIT_SUCCESS;
}