#include <stdio.h>
#include <QObject>
#include <QUrl>
#include <QFile>
#include <iostream>
#include <QTime>

#include <QCoreApplication>
#include <cstdio>
#include <pthread.h>
#include <QThread>
#include <QDebug>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>
#include <wiringPi.h>

#include <termios.h> //gethch
#include <linux/input.h>

#include <unistd.h>
#include <pthread.h>
#include "gpio.h"

#include <QJsonDocument>
#include <QNetworkReply>
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QThread>
#include <QIODevice>

#include "httprequest.h"
#include "rgb_lcd.h"
#include "config.h"
#include "my_lcd.h"

#include <QElapsedTimer>

HttpRequest::HttpRequest(): QObject()
{
    pthd.count=0; // struct pthread_data co 3 bien count, counting, stop_count
    pthd.counting=0;
    pthd.stop_count=0;
    reply = nullptr;
    httpRequestAborted = false;
    resetParam();
    writeLog("Start KinderGarden!!!");
//    connect(this, SIGNAL(callcheck()), this, SLOT(check()));
}
void HttpRequest::writeLog(QString mess)    /* write to file */
{
    QString fileName = QDate::currentDate().toString("yyyy_MM_dd"); /* get current date */
    QString fileUrl = "/home/pi/logs/Adela_QtVersion/Log/" + fileName + ".txt";   /* get file name is current day */

    QFile file(fileUrl);    /* Create a file with file name is current date */
    if (!file.open(QIODevice::Append | QIODevice::Text))    /* QIODevice:: mode*/
        /*mode Append: Data is writen at the end of file
         *mode Text: read: eol is '\n', but write: eol is '\r\n' */
        return;

    QTextStream in(&file);  /* Constructor a stream with file */
    if (in.atEnd()) /* Returns true if there is no more data to be read from the QTextStream; otherwise returns false */
        in << QTime::currentTime().toString("hh:mm:ss")<<" "<< mess << "\n";    /* write to file with mess and time*/
}

//ham nhan ma nhap vao, xu ly cac nut nhan
void HttpRequest::getCode()
{
    static pthread_t tid;
    char c=getch();

    if((c>='0') && (c<='9')){
        key[pthd.count] = c;
//        setCursor(pthd.count, 1);
//        lcd_write(c);
        pthd.count ++;        
        if(pthd.count >= 16) pthd.count = 0;
        if(pthd.counting) pthd.stop_count = 1;
    }
    else{   /* Cac phim chuc nang  */
        switch (c) {
        case 0:

            break;
        //page up
        case 151:
            break;
        case 156:
            break;
        case 58:    //esc
            pthread_cancel(tid);
            pthd.count = 0;
            if(pthd.counting) pthd.stop_count = 1;
//            my_lcd_clear_line(1);
            qDebug() <<"stop_count" << (pthd.stop_count) << "\n";
            qDebug() << (pthd.counting);
            qDebug() << (pthd.count);
            break;
        case 61:    //clear
            pthread_cancel(tid);
            pthd.count = 0;
            if(pthd.counting) pthd.stop_count = 1;
//            my_lcd_clear_line(1);
            break;
        case 75:    //Enter
            pthd.count = 0;
            isInputting = false;
            QString keyString = key;
            writeLog("==============");
            writeLog("<input> " + keyString);
            code = keyString;
            for (int x = 0; x < 9 ; x ++) {
                key[x] = 0;
            }
//            my_lcd_clear_line(1);
            if(code == ""){
                qDebug() << "Nhap code truoc khi enter";
//                setCursor(0,1);
//                lcd_write_string("Please Type code");
                delay(1000);
//                my_lcd_clear_line(1);
                isInputting = true;
            }
            else{
                if(pthd.counting) pthd.stop_count = 1;
                if(checkStaff(code) == true){
                    writeLog("staff unlock");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("Hello");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() << "staff unlock";
                    unlockLock1();
                    lock1IsOpenning = true;
                    isInputting = true;
                }   /* End checkStaff(code) == true */
                else if(code == "27042021"){
                    writeLog("Unlocked all");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("Unlock all");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() <<"Unlock";
                    unlockLock1();
                    lock1IsOpenning = true;
                    unlockLock2();
                    lock2IsOpenning = true;
                    isInputting = true;
                }   /* End code == 9999... */
                else if(code == "27042222"){
                    writeLog("Locked all");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("Lock all");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() <<"Locked all";
                    gpio_lock(lock1Pin);
                    gpio_lock(lock2Pin);
                    lock1IsOpenning = false;
                    lock2IsOpenning = false;
                    isInputting = true;
                }   /* End code == 000... " */
                else if(code == "27045555"){
                    writeLog("Unlocked KID");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("unlock kid room");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() << "unlock Kid";
                    unlockLock2();
                    lock2IsOpenning = true;
                    isInputting = true;
                }
                else if(code == "27048888"){
                    writeLog("Locked KID");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("Lock kid room");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() << "lock Kid";
                    gpio_lock(lock2Pin);
                    lock2IsOpenning = false;
                    isInputting = true;
                }
                else if(code == "27046666"){
                    writeLog("Unlocked Station");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("unlock station");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() << "unlocked station";
                    unlockLock1();
                    lock1IsOpenning = true;
                    isInputting = true;
                }
                else if(code == "27049999"){
                    writeLog("Locked station");
//                    my_lcd_clear_line(1);
//                    setCursor(0,1);
//                    lcd_write_string("lock station");
//                    delay(1000);
//                    my_lcd_clear_line(1);
                    qDebug() << "lock station";
                    gpio_lock(lock1Pin);
                    lock1IsOpenning = false;
                    isInputting = true;
                }
                else {
                    check();
    //                    checkCheck(code);
                }   /* End mode online */
            }
            break;
        }
    }
}

//Get tu ban phim, bat su kien nhan phim
char HttpRequest::getch()
{
    struct input_event ev[64];
    int size = sizeof (struct input_event);
    ssize_t n;
    int file_keyboard;
    char* filename = (char*)"/dev/input/event0";
    for (;;)
    {

        //----- OPEN THE KEYBOARD device -----
        if ((file_keyboard = open(filename, O_RDONLY)) < 0)
        {
            printf("Failed to open the keyboard ");
            return 0;
        }
        n = read(file_keyboard, &ev, size * 64);

        close(file_keyboard);
        if ((n!=0) &&
                (ev[0].type == 4) &&
                (ev[1].value == 1)){
            //            printf(". %d .\n", ev[1].code);
//            qDebug() << (ev[1].code - 1 + '0') <<  " k,\n";
            if((ev[1].code - 1 + '0') == 48) return 58;     /* neu ev code = 1 --> ESC thi return key ':' */
            if((ev[1].code - 1 + '0') == 58) return 48;     /* Neu nhap key = 11( phim 0) --> return '0'*/
            /* Check mã nhập vào */
            return (ev[1].code - 1 + '0');  /* return phim nhap vao */
        }
    }
}

bool HttpRequest::checkStaff(QString codeToCheck)
{
    //Mo file. doc tung line sau do check voi codeToCheck
    QString filename = "/home/pi/logs/Adela_QtVersion/staff/staff_code.txt";
    QFile file(filename);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
        qDebug() << "Error while open file\n";
        return false;
    }
    //So sanh voi line trong file
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if(QString::compare((codeToCheck + QString("\n")), line) == 0){    //EQUAL
            //So sanh voi code trong file, neu bang--OK
            file.close();
            return true;
        }
    }
    //Duyet het file ma ko thay ong nao khop-->false
    file.close();
    return false;

}

void HttpRequest::writeCodeToFileCheck(QString codeCheck)
{
    QString fileName = QDate::currentDate().toString("yyyy_MM_dd");
    QString fileUrl = "/home/pi/logs/Adela_QtVersion/code/" + fileName +".txt";

    QFile file(fileUrl);
    if(!file.open(QIODevice::Append | QIODevice::Text)){
        /*mode Append: Data is writen at the end of file
         *mode Text: read: eol is '\n', but write: eol is '\r\n' */
        qDebug() << "Error while open file\n";
        return;
    }
    QTextStream in(&file);
    if(in.atEnd()){
        in << codeCheck <<"\n";
        qDebug() << "Writed "<< codeCheck << " to file";
    }
    file.close();

    return;
}

int HttpRequest::checkCheck(QString codeCheck)
{
    QString fileName = QDate::currentDate().toString("yyyy_MM_dd");
    QString fileUrl = "/home/pi/logs/Adela_QtVersion/code/" + fileName +".txt";

    QFile file(fileUrl);
    if(!file.open(QIODevice::ReadOnly)){
//        qDebug() << "Error while open file" << fileName;
//        return -1;
        file.open(QIODevice::NewOnly);
        checkCheck(codeCheck);
    }
    QString line;
    QTextStream in(&file);
    while(!in.atEnd()){
        line = in.readLine();
        if(((codeCheck == line))){
            file.close();
            return CHECKOUT;
        }
    }
    file.close();
    return CHECKIN;
}

void HttpRequest::startRequest()
{
    // gui yeu cau http request
//    my_lcd_clear_line(0);
//    my_lcd_clear_line(1);
//    setCursor(0,0);
//    lcd_write_string("waiting....");
    qnam = new QNetworkAccessManager;
    writeLog("waiting....");
    httpRequestAborted = false;

    qDebug() << url;

    QNetworkRequest request(url); // yeu cau URL
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));// dinh dang noi dung yeu cau la file json
    QByteArray data;
    qDebug() <<code;
    data.append("{\"code\": \""+code+"\"}");
    qDebug() <<data;
    reply = qnam->post(request,data); // post yeu cau len de lay noi dung cua yeu cau dich tra ve 1 Qnetwork reply
    connect(reply, &QNetworkReply::finished, this, &HttpRequest::httpFinished);// hoan tat qua trinh xu ly
    connect(reply, &QIODevice::readyRead, this, &HttpRequest::httpReadyRead);
}

void HttpRequest::resetParam()
{
    isRequestPrevSuccess = true;
    res_isValid = 0;
    res_link = "";
    res_room_id = "";
}

void HttpRequest::httpFinished()
{
    qDebug() << "finished";
//    // yeu cau bi huy
////    if (httpRequestAborted) {
////        reply->deleteLater();
////        reply = nullptr;
////        return;
////    }
////    // tra loi loi
////    if (reply->error()) {
////        qDebug()<<reply->errorString();
////        reply->deleteLater();
////        reply = nullptr;
////        resetParam();
////        startNewSession();
////    }
//    reply->deleteLater();
//    reply = nullptr;
//    // QTimer::singleShot(25000, this, SLOT(onWaitLock()));
//    resetParam();
//    startNewSession();
}

void HttpRequest::httpReadyRead()
{
    qDebug() << "Ready read";
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant contentLengthV = reply->header(QNetworkRequest::ContentLengthHeader);

    QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

    qDebug() <<"statuscode: "<< statusCodeV.toString();
    qDebug() <<"contentLength: "<< contentLengthV.toString();

    QByteArray rep = reply->readAll();
    qDebug()<<"rep   "<<rep;

    QJsonDocument response = QJsonDocument::fromJson(rep);
    QString strFromObj = QJsonDocument(response).toJson(QJsonDocument::Compact).toStdString().c_str();
    qDebug()<<"jsonn: " << strFromObj;

    if (statusCodeV.toString() == "200") {
//        res_room_id = response.object().value("room_id").toString();
//        if (res_room_id == "1")
        writeLog("user open");
        writeLog(code);
        unlockLock2();
        qDebug() << "unlocked KID";
//        my_lcd_clear_line(0);
//        my_lcd_clear_line(1);
//        setCursor(0,1);
//        lcd_write_string("OK");
        lock2IsOpenning = true;
        if(checkCheck(code) == CHECKIN)
        writeCodeToFileCheck(code);
        //ghi len LCD 2s
//        startNewSession();
    }
    else
    {
        writeLog("Something wrong!");
        qDebug("Code sai");
        //ghi len lcd 2s
//        my_lcd_clear_line(0);
//        my_lcd_clear_line(1);
//        setCursor(0,0);
//        lcd_write_string("Something wrong!");
///
    }
    reply->deleteLater();
    delay(1000);
    resetParam();
    startNewSession();
}

void HttpRequest::check()
{
        writeLog("API check");
        if (checkCheck(code) == CHECKIN)
        {
            writeLog("CHECKIN");
            url = QString("http://api.adela.com.vn/api/children_rooms/checkin");
            //+QString("?code=" + code);
            qDebug() << url;
            startRequest();
        }
        else if(checkCheck(code) == CHECKOUT){
            writeLog("CHECKOUT");
            url = QString("http://api.adela.com.vn/api/children_rooms/checkout");
                    //+QString("?code=" + code);
            qDebug() << url;
            qDebug() <<"CHECKOUT";
            startRequest();
        }
        else {
            qDebug() << "CHECK SAI";
            startNewSession();
        }
}

void HttpRequest::start()
{
    startNewSession();
}

void HttpRequest::startNewSession()
{
//    my_lcd_clear_line(0);
//    my_lcd_clear_line(1);
//    setCursor(0, 0);
//    lcd_write_string("Enter key: ");
    resetParam();
    code = "";
    isInputting = true;
    qDebug()<< "Enter code: ";
    while (isInputting) {
        getCode();
    }
}
