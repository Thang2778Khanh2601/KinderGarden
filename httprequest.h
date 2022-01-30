#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

#define ONLINE true
#define OFFLINE false
#define CHECKIN 1
#define CHECKOUT 2

extern int8_t openLock1;
extern int8_t openLock2;
extern bool lock1IsOpenning;
extern bool lock2IsOpenning;

struct pthread_data{    /* luong du lieu */
   uint8_t  count;  /*dem*/
   uint8_t counting;    /*dang dem*/
   uint8_t stop_count;  /*dung dem*/
};


class HttpRequest : public QObject
{
    Q_OBJECT
public:
    HttpRequest();

    void writeLog(QString mess);
    void getCode();         /* nhận mã, các trường hợp như pageup, down... ,., enter... */
    static char getch();    /* get từ phím */
    bool checkStaff(QString codeToCheck);
    void writeCodeToFileCheck(QString code);
    int checkCheck(QString code);

    /* HTTP */
    void start();
    void startNewSession();
    void startRequest();
    void resetParam();

private slots:
    void httpFinished();
    void httpReadyRead();
    void check();
signals:
    void callcheck();

private:
    bool mode = ONLINE;
    char key[8] = {0};
    bool isInputting;
    struct pthread_data pthd;
    int count;
    int counting;
    int stop_count;
    QString code = "";

    bool isRequestPrevSuccess;
//    int statusRequest;
    QUrl url;
    QString link;

    // resetParam
    int res_isValid;
    QString res_link;
    QString res_room_id;

    QNetworkAccessManager *qnam; // cho phep gui cac yeu cau mang va nhan tra loi
    QNetworkReply *reply;
    bool httpRequestAborted; // yeu cau bi huy bo
};

#endif // HTTPREQUEST_H
