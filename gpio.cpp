#include <wiringPi.h>
#include "config.h"
#include <stdio.h>
#include "gpio.h"

int8_t openLock1 = -1;
int8_t openLock2 = -1;

bool lock1IsOpenning = false;
bool lock2IsOpenning = false;

pthread_mutex_t mutex;
pthread_cond_t runThread1;
pthread_cond_t runThread2;


bool isStaff = false;

void gpio_init(){
    pinMode(lock1Pin, OUTPUT);
    pinMode(exit1Pin, INPUT);
    pinMode(lock2Pin, OUTPUT);
    pinMode(exit2Pin, INPUT);

    //
    gpio_lock(lock1Pin);
    gpio_lock(lock2Pin);
}

void* unlock1Thread(void *arg){
    switch (openLock1) {
    case 1:
        //Mở khóa 1, đợi 15s sau đó đưa biến exitIspressed về -1;
        gpio_unlock(lock1Pin);
        lock1IsOpenning = true;
//            printf("Exit1Pressed = : %d \n", openLock1);
        for (int i = 0; i < 15; i ++) {
//                printf("From lock 1: %d\n", i);
            delay(1000);
        }
        if(lock1IsOpenning == true){
            gpio_lock(lock1Pin);
            lock1IsOpenning = false;
        }
        openLock1 = -1;
        break;
    default:
        break;
    }
    return nullptr;
}

void* unlock2Thread(void *arg){
    switch (openLock2) {
    case 1:
        //Mở khóa 1, đợi 15s sau đó đưa biến exitIspressed về -1;
        gpio_unlock(lock2Pin);
        lock2IsOpenning = true;
//            printf("Exit2Pressed = : %d \n", openLock2);
        for (int i = 0; i < 15; i ++) {
//                printf("From lock 2: %d\n", i);
            delay(1000);
        }
        if(lock2IsOpenning == true){
            gpio_lock(lock2Pin);
            lock2IsOpenning = false;
        }
        openLock2 = -1;
        break;
    default:
        break;
    }
    return nullptr;
}

void gpio_unlock(uint8_t ledPin){
    printf("lock %d mo\n", ledPin);
    digitalWrite(ledPin, HIGH);
}

void gpio_lock(uint8_t ledPin){
    printf("lock %d khoa\n", ledPin);
    digitalWrite(ledPin, LOW);
}
/* handler ISR 1 */
void ISR_exit1(){
    unlockLock1();
    delay(200);
}
/* handler ISR 2 */
void ISR_exit2(){
    unlockLock2();
    delay(200);
}

void unlockLock1(){
    openLock1 = 1;
    pthread_t lock1ThreadID;
    pthread_create(&lock1ThreadID, nullptr, &unlock1Thread, nullptr);
}

void unlockLock2(){
    openLock2 = 1;
    pthread_t lock2ThreadID;
    pthread_create(&lock2ThreadID, nullptr, &unlock2Thread, nullptr);
}
