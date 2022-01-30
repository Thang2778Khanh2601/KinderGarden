#include <QCoreApplication>
#include <wiringPi.h>
#include <QDebug>
#include "gpio.h"
#include "config.h"
#include "httprequest.h"
#include <pthread.h>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /* Set up wiringPi and ISR */
    if(wiringPiSetup() == -1){
        printf("Failed open wiringPi\n");
    }
    /* Turn on ISR*/
    if(wiringPiISR(exit1Pin, INT_EDGE_FALLING, &ISR_exit1) < 0){
        printf("setup ISR failed\n");
    }
    if(wiringPiISR(exit2Pin, INT_EDGE_FALLING, &ISR_exit2) < 0){
        printf("setup ISR failed\n");
    }

    HttpRequest testGetCode;
//    testGetCode.writeCodeToFileCheck("begin raspberry pi");
    testGetCode.start();
//    pthread_join(lockThreadID, nullptr);
//    pthread_join(lock2ThreadID, nullptr);

    return a.exec();
}




