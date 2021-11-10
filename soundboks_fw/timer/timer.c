#include "timer.h"
#include <limits.h>
#include <stdbool.h>

// ** Table of `MAX_N_SOFT_TIMERS` soft_timers
static Timer_t soft_timers[MAX_N_SOFT_TIMERS] =
{
    {0UL, 0UL, {0}, NULL, NULL}
};
static size_t last_i = 0;

inline Timer_t * timer_start(uint32_t period, 
             bool periodic, 
             timer_callback_t clbk,
             void * arg) {
    //Find first available timer
    size_t i;
    for(i = last_i;i < MAX_N_SOFT_TIMERS; ++i) {
        if(!soft_timers[i].flags.running) {
            soft_timers[i].period = period;
            soft_timers[i].flags.running = true;
            soft_timers[i].flags.periodic = periodic;
            soft_timers[i].clbk = clbk;
            soft_timers[i].arg = arg;
            soft_timers[i].stime = get_clock();
            return &soft_timers[i];
        }
    }
    return NULL; //No available timers
}


inline void timer_stop(Timer_t * tmr) {
    tmr->flags.running = 0;
}

inline bool is_running(Timer_t * tmr) {
    return tmr->flags.running;
}

void update_timers() {
    uint32_t ctime;
    for(size_t i = 0;i < MAX_N_SOFT_TIMERS; ++i) {
        if(soft_timers[i].flags.running) {
            ctime = get_clock();
            uint32_t delta;
            if(soft_timers[i].stime > ctime) {
                delta = ULONG_MAX - ctime + soft_timers[i].stime;
            }
            else {
                delta = ctime - soft_timers[i].stime;
            }
            
            if(delta > soft_timers[i].period) {
                soft_timers[i].flags.running = soft_timers[i].flags.periodic;
                if(!soft_timers[i].flags.running) {
                    last_i = i;
                }
                soft_timers[i].stime = ctime;
                soft_timers[i].clbk(soft_timers[i].arg);
            }
        }
    }
}
