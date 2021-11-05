#ifndef EVENTS_H
#define EVENTS_H

#include <stddef.h>

typedef unsigned long Event_t;

typedef enum {
    EVENT_NONE = 0x0UL,
    EVENT_INIT
} basic_evt_t;

#define LAST_EVENT EVENT_INIT; //useful to extend the events table

typedef struct {
    Event_t * buf;
    size_t len,
            cap,
            wptr,
           rptr;
} Evt_que_t, *Evt_que_tp;

extern Evt_que_t mk_evt_que(size_t, Event_t*);
extern int put_evt(Evt_que_tp, Event_t);
extern int get_evt(Evt_que_tp, Event_t *);

#endif //EVENTS_H