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
    struct {
        uint32_t running : 1;
        uint32_t periodic : 1;
    } flags;
    timer_callback_t clbk;
    void * arg;
} Timer_t;


/**  @brief A function that starts a soft-timer from a table of timers. 
*
* \details The function first looks for a non-running timer in a table of software timers, and starts this timer.
* This timer can be one time, i.e. it expires only once, or periodic, in which case it expires indefinitely,
* until stopped manually. A callback function is called upon expiration.

* @param period The period, or expire time, of the timer
* @param periodic Setting this to false will start a one-time timer; `true` starts a periodic timer.
* @param clbk A pointer to a callback function
* @param arg A pointer to an argument that is passed to the callback function
* @return Returns a pointer to an available timer after it starts; returns NULL if no timer is available.
*/
extern Timer_t * timer_start(uint32_t period, 
             bool periodic, 
             timer_callback_t clbk,
             void * arg);
/**
 * @brief Stops a timer.
 * 
 * @param tmr The pointer handle to a timer to be stopped.
 */
extern void timer_stop(Timer_t * tmr);  
/**
 * @brief Tests if a timer is running.
 * 
 * @param tmr The pointer handle to a timer to be stopped.
 * @return `true` whenever the timer is running, false otherwise.
 */
extern bool is_running(Timer_t * tmr);   
/**
 * @brief Function to update the timers, based on the global timer reference.
 * @details This function takes care that the timers are compared with a reference clock,
 *   and a callback is called if the time delta is greater than the setup period. If a
 *   timer is not periodic, then this function also stops the timer, by setting its
 *   running flag to false. This function should be periodically called by an external process.
 *   This means that the callbacks are executed within the thread of this process.
 */        
extern void update_timers();
/**
 * @brief Gets the current time in clock cycles.
 * @details This function is a stub, and must be implemented by the user. The function
 *   must return the elapsed time in clock cycles from system start-up. 
 * @return The clock cycle count 
 */
extern uint32_t get_clock();

#endif // TIMER_H