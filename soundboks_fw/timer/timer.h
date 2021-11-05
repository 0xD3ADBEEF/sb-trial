#ifndef TIMER_H
#define TIMER_H

#include <stddef.h>

typedef int (*timer_callback_t)(void *ctx);

int callback_register(timer_callback_t cb, size_t time, void *ctx);

#endif // TIMER_H