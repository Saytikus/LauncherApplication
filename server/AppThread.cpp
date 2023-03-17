#include "AppThread.h"

AppThread::AppThread(QObject *parent) : QObject{parent} {
    thread_ = new QThread();
    thread_->setObjectName(QString::number(thread_count_)); //тест
}

void AppThread::Start() {
    thread_->start();
}

void AppThread::MoveToThread(QUdpSocket *socket) {
    socket->moveToThread(thread_);
}

void AppThread::IncreaseThreadCount() {
    thread_count_++;
}
