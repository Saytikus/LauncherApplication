#include "DataHandlerThreadPool.h"

DataHandlerThreadPool::DataHandlerThreadPool(QObject *parent) : QObject{parent} {

}

void DataHandlerThreadPool::Start() {
    for(int i = 0; i < max_thread_count_; i++) {
        QThread *thread = new QThread();
        ++current_thread_count_;
        thread->setObjectName("Handler thread №" + QString::number(current_thread_count_));

        DataHandler *handler = new DataHandler();
        handler->moveToThread(thread);

        //connect(thread, SIGNAL(finished), handler, SLOT(deleteLater));

        threads.push_back(thread);
        handlers.push_back(handler);

        thread->start();
    }
}

void DataHandlerThreadPool::DeleteHandler(QThread *thread) {
    thread->quit();
    thread->wait();
}

void DataHandlerThreadPool::ReadBufferChange(const QBuffer *read_buffer) {
    qDebug() << "Поток в ReadBufferChange: " << QThread::currentThread(); //
    qDebug() << "Данные read_buffer в ReadBufferChange: " << read_buffer->data();
    DataHandler *handler = handlers[rr_count_ % max_thread_count_];
    ++rr_count_;

    qRegisterMetaType<const QBuffer*>("const QBuffer*");

    QMetaObject::invokeMethod(handler, "HandleClientRequest",
                Qt::QueuedConnection,
                Q_ARG(const QBuffer*, read_buffer));
}

void DataHandlerThreadPool::SetMaxThreadCount(const int thread_count) {
    max_thread_count_ = thread_count;
}
