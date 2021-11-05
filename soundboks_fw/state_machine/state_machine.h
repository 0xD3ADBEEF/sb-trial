#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "events.h"

typedef int (*state_callback_t)(void *arg);

typedef struct {
    uint32_t id;
    state_callback_t clbk;
} State_t, * State_tp;

typedef struct {
    Event_t evnt; 
    State_tp from, to;
} Trans_t, * Trans_tp;

typedef struct {
    State_tp state;
    size_t ntrans;
    Trans_tp trans;
    Evt_que_tp que;
} State_machine_t, * State_machine_tp;

extern void execute(State_machine_tp);
extern Trans_t mk_trans(Event_t, State_tp from, State_tp to);
extern State_machine_t mk_stm(State_tp, size_t, Trans_tp, Evt_que_tp);

#endif //STATE_MACHINE_H