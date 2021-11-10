#include "events.h"

Evt_que_t mk_evt_que(size_t len, Event_t * buf) {
    Evt_que_t evt_que = {
        buf, len, 0, 0, 0
    };
    return evt_que;
}

inline bool is_empty(Evt_que_tp que) {
    return que->cap == 0;
}
inline bool is_full(Evt_que_tp que) {
    return que->cap == que->len;
}
inline Event_t get_head(Evt_que_tp que) {
    if(is_empty(que)){
        return EVENT_NONE;
    }
    else {
        return que->buf[que->rptr];
    }
}
inline bool has_evt(Evt_que_tp que, Event_t evt) {
    return !is_empty(que) && evt == get_head(que);
}

int put_evt(Evt_que_tp que, Event_t evt) {
    if(que->cap == que->len || evt == EVENT_NONE) {
        return 1; //full/can't put event
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

int pop_evt(Evt_que_tp que) {
    if(que->cap == 0) {
        return 2; //empty
    }
    --que->cap;
        
    if(++que->rptr == que->len) {
        que->rptr = 0;
    }
    return 0; //Success
}