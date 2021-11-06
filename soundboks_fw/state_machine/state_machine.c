#include "state_machine.h"
#include <stddef.h>

inline State_t mk_state(uint32_t id, state_callback_t clbk, void * arg) {
    State_t state = {
        id, clbk, arg
    };
    return state;
}

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
    State_machine_t stm = {
        init, ntrans, trans, que
    };
    init->clbk(init->arg);
    return stm;                                  
}

static int transition(const State_machine_tp stm) {
    const State_tp st = stm->state;
    Trans_tp trans = stm->trans;

    for(size_t i = 0; i < stm->ntrans; ++i) {
        if(st == trans[i].from) {
            if(trans[i].evnt == EVENT_NONE) { //Not subscribed to event, transition
                stm->state = trans[i].to;
                return 0;
            } else if(trans[i].evnt == get_head(stm->que)) {
                pop_evt(stm->que);
                stm->state = trans[i].to;
                return 0;
            }
        }
    }
    return 1;
}

void execute(const State_machine_tp stm) {
    //Transition to the next state, based on the event
    if(!transition(stm)) { //If we transition, do something
        if( stm->state->clbk != NULL ) {
            stm->state->clbk(stm->state->arg); //Call the callback function
        }
    }
}