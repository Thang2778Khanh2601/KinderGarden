#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <QObject>

    struct pthread_data{    /* luong du lieu */
       uint8_t  count;  /*dem*/
       uint8_t counting;    /*dang dem*/
       uint8_t stop_count;  /*dung dem*/
    };

    void getCode();         /* nhận mã, các trường hợp như pageup, down... ,., enter... */
    char getch();    /* get từ phím */

    char key[9];
    bool isInputting;
    struct pthread_data pthd;
    int count;
    int counting;
    int stop_count;
    QString code;

#endif // KEYBOARD_H
