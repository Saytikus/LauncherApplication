QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ApplicationDatabase.cpp \
    ClientBuffer.cpp \
    ClientBufferPool.cpp \
    Combiner.cpp \
    DataHandler.cpp \
    DataHandlerThreadPool.cpp \
    DatabaseWindow.cpp \
    MainWindow.cpp \
    QUdpServer.cpp \
    Sender.cpp \
    main.cpp

HEADERS += \
    ApplicationDatabase.h \
    ClientBuffer.h \
    ClientBufferPool.h \
    Combiner.h \
    DataHandler.h \
    DataHandlerThreadPool.h \
    DatabaseWindow.h \
    MainWindow.h \
    QUdpServer.h \
    Sender.h

FORMS += \
    DatabaseWindow.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
