#include "AppThread.h"

AppThread::AppThread(QObject *parent) : QObject{parent} {
    QObject sender;
    QThread thread;
    thread.setObjectName("Sender");
    sender.moveToThread(&thread);

}
