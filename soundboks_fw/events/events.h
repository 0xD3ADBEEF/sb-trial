#ifndef EVENTS_H
#define EVENTS_H

#include <stddef.h>
#include <stdbool.h>

typedef unsigned long Event_t;

typedef enum {
    EVENT_NONE = 0x0UL
} basic_evt_t;

#define EVENT_LAST EVENT_NONE //useful to extend the events table

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
extern int pop_evt(Evt_que_tp que);
extern bool is_empty(Evt_que_tp que);
extern bool is_full(Evt_que_tp que);
extern Event_t get_head(Evt_que_tp que);
extern bool has_evt(Evt_que_tp, Event_t);

#endif //EVENTS_H