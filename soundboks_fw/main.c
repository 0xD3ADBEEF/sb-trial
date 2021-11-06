#include "timer.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum { //Test
    STATE_INIT,
    STATE_ACTIVE,
    STATE_PASSIVE,
} main_state_t;

bool stop = false;
int cb(void *ctx) {
    printf("Tick\n");
    // stop = true;
    return 0;
}

int cb2(void * arg) {
    Timer_t * tmr = (Timer_t*)arg;
    printf("Tock\n");
    timer_stop(tmr);
    // stop = true;
    return 0;
}


int main(void) {
    Timer_t * tmr;
    clock_t t1, t2;
    double cpu_time_used;

    // static main_state_t state = STATE_INIT;
    // Include examples on how the timer can be used here in the main function

    // switch (state)
    // {
    // case STATE_INIT:
    //     callback_register(&cb, 1, NULL);
    //     /* Init stuff here */
    //     state = STATE_ACTIVE;
    //     break;
    
    // case STATE_ACTIVE:
    //     /* Do active part here */
    //     state = STATE_PASSIVE;
    //     break;
    
    // case STATE_PASSIVE:
    //     /* Do passive part here */
    //     break;
    
    // default:
    //     /* Default handler should not be hit in this case */
    //     break;
    // }

    tmr = timer_start(TIME_SEC(1.0), true, cb, NULL);
    timer_start(TIME_SEC(5.1), false, cb2, tmr);

    t1 = clock();
    while(is_running(tmr)) {
        //Do stuff
        update_timers();
    }
    t2 = clock();
    cpu_time_used = (double)(t2 - t1)/CLOCKS_PER_SEC; 
    printf("Elapsed time: %.5fs\n", cpu_time_used);

    return EXIT_SUCCESS;
}