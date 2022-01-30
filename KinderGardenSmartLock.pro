QT -= gui

CONFIG += c++11 console
QT += network
QT += core
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib -lwiringPi

SOURCES += \
        gpio.cpp \
        httprequest.cpp \
        main.cpp \
        my_lcd.cpp \
        rgb_lcd.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    config.h \
    gpio.h \
    httprequest.h \
    my_lcd.h \
    rgb_lcd.h

DISTFILES += \
    staff_code.txt
