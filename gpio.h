#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
#include <pthread.h>
#include "config.h"
#ifdef __cplusplus
extern "C" {
#endif

extern bool isStaff;
extern int8_t openLock1;
extern int8_t openLock2;
extern bool lock1IsOpenning;
extern bool lock2IsOpenning;

void gpio_init();
void gpio_lock(uint8_t ledPin);
void gpio_unlock(uint8_t ledPin);
void* lock1Thread(void *arg);
void* lock2Thread(void *arg);
void ISR_exit1();
void ISR_exit2();
void unlockLock1();
void unlockLock2();

#ifdef __cplusplus
}
#endif

#endif // GPIO_H
