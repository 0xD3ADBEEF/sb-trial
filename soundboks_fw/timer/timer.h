#ifndef TIMER_H
#define TIMER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MAX_N_SOFT_TIMERS
#define MAX_N_SOFT_TIMERS 5
#endif

#define TIME_SEC(sec) ((uint32_t)(sec*CLOCKS_PER_SEC))

typedef int (*timer_callback_t)(void *ctx);

typedef struct {
    uint32_t period,
            stime;
    bool running,
         periodic;
    timer_callback_t clbk;
    void * arg;
} Timer_t;

extern void timer_stop(Timer_t * tmr);
extern bool is_running(Timer_t * tmr);
extern Timer_t * timer_start(uint32_t period, 
             bool periodic, 
             timer_callback_t clbk,
             void * arg);
extern void update_timers();
extern uint32_t get_clock();
// extern void print_timers();
// int callback_register(timer_callback_t cb, size_t time, void *ctx);

#endif // TIMER_H