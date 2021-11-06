#include "timer.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

static Timer_t soft_timers[MAX_N_SOFT_TIMERS] =
{
    {0UL, 0UL, false, false, NULL, NULL}
};

// void print_timers() {
//     for(size_t i = 0;i < MAX_N_SOFT_TIMERS; ++i) {
//         printf("%lu, %lu, %d\n",soft_timers[i].timeout,
//                             soft_timers[i].stime,
//                             soft_timers[i].running);
//     } 
// }

inline Timer_t * timer_start(clock_t period, 
             bool periodic, 
             timer_callback_t clbk,
             void * arg) {
    //Find first available timer
    size_t i;
    for(i = 0;i < MAX_N_SOFT_TIMERS; ++i) {
        if(!soft_timers[i].running) {
            soft_timers[i].period = period;
            soft_timers[i].running = true;
            soft_timers[i].periodic = periodic;
            soft_timers[i].clbk = clbk;
            soft_timers[i].arg = arg;
            soft_timers[i].stime = clock();
            return &soft_timers[i];
        }
    }
    return NULL; //No available timers
}

inline void timer_stop(Timer_t * tmr) {
    tmr->running = false;
}

inline bool is_running(Timer_t * tmr) {
    return tmr->running;
}

void update_timers() {
    clock_t end;
    for(size_t i = 0;i < MAX_N_SOFT_TIMERS; ++i) {
        if(soft_timers[i].running) {
            end = clock();
            clock_t delta;
            if(soft_timers[i].stime > end) {
                delta = ULONG_MAX - end + soft_timers[i].stime;
            }
            else {
                delta = end - soft_timers[i].stime;
            }
            
            if(delta > soft_timers[i].period) {
                soft_timers[i].running = soft_timers[i].periodic;
                soft_timers[i].stime = end;
                soft_timers[i].clbk(soft_timers[i].arg);
            }
        }
    }
}

int callback_register(timer_callback_t cb, size_t time, void *ctx) {
    //@TODO: Implement
    return 0;
}