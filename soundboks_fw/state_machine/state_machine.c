#include "state_machine.h"
#include <stddef.h>

inline Trans_t mk_trans(Event_t evt, State_tp from, State_tp to) {
    Trans_t trans = {
        evt, from, to
    };
    return trans;
}

inline State_machine_t mk_stm(State_tp init, 
                              size_t ntrans, 
                              Trans_tp trans, 
                              Evt_que_tp que) {
    put_evt(que, EVENT_INIT);                                  
    State_machine_t stm = {
        init, ntrans, trans, que
    };
    return stm;                                  
}

static Event_t consume(const State_machine_tp stm) {
    Event_t evt;
    get_evt(stm->que, &evt);
    return evt; 
}

static int transition(const State_machine_tp stm, 
                       const Event_t evnt) {
    const State_tp st = stm->state;
    Trans_tp trans = stm->trans;

    for(size_t i = 0; i < stm->ntrans; ++i) {
        if(st == trans[i].from && evnt == trans[i].evnt) {
            stm->state = trans[i].to;
            return 0;
        }
    }
    return 1;
}

void execute(State_machine_tp stm) {
    Event_t evnt = consume(stm); //Consume event

    //Transition to the next state, based on the event
    if(evnt == EVENT_INIT || !transition(stm, evnt)) { //If we transition, do something
        if( stm->state->clbk != NULL ) {
            stm->state->clbk((void*)stm); //Call the callback function
        }
    }
}