#include "events.h"

Evt_que_t mk_evt_que(size_t len, Event_t * buf) {
    Evt_que_t evt_que = {
        buf, len, 0, 0, 0
    };
    return evt_que;
}

int put_evt(Evt_que_tp que, Event_t evt) {
    if(que->cap == que->len) {
        return 1; //full
    }
    que->buf[que->wptr] = evt;
    ++que->cap;
        
    if(++que->wptr == que->len) {
        que->wptr = 0;
    }
    return 0; //Success
}

int get_evt(Evt_que_tp que, Event_t * evt) {
    if(que->cap == 0) {
        *evt = EVENT_NONE;
        return 2; //empty
    }
    *evt = que->buf[que->rptr];
    --que->cap;
        
    if(++que->rptr == que->len) {
        que->rptr = 0;
    }
    return 0; //Success
}