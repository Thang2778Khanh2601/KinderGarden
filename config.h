#ifndef CONFIG_H
#define CONFIG_H
#include <pthread.h>
#include <stdint.h>

extern pthread_mutex_t mutex;
extern pthread_cond_t runThread1;
extern pthread_cond_t runThread2;

#define lock1Pin 25 /* Wpi25 = GPIO 26 */
#define lock2Pin 23 /* Wpi23 = GPIO 13 */
#define exit1Pin 2  /* Wpi0  = GPIO 17 */
#define exit2Pin 0  /* Wpi2  = GPIO 27 */

#endif // CONFIG_H
