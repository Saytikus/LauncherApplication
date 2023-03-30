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
        connect(handler, SIGNAL(RequestCreateProfileToPool(const QString, const QString, const QString)),
                this, SLOT(TransferRequestCreateProfile(const QString, const QString, const QString)));
        connect(handler, SIGNAL(RequestExistsProfileToPool(const QString, const QString, const QString)),
                this, SLOT(TransferRequestExistsProfile(const QString, const QString, const QString)));
        connect(handler, SIGNAL(SendCompleteAnswerToPool(const QByteArray, const int, const quint16)),
                this, SLOT(TransferSendCompleteAnswer(const QByteArray, const int, const quint16)));

        connect(this, SIGNAL(SendAnswerCreateProfileToHandler(const int)),
                handler, SLOT(HandleAnswerCreateProfile(const int)));
        connect(this, SIGNAL(SendAnswerExistsProfileToHandler(const int)),
                handler, SLOT(HandleAnswerExistsProfile(const int)));

        threads.push_back(thread);
        handlers.push_back(handler);

        thread->start();
    }
}

void DataHandlerThreadPool::DeleteHandler(QThread *thread) {
    thread->quit();
    thread->wait();
}

void DataHandlerThreadPool::ReadBufferChange(const QBuffer *read_buffer, const quint16 buffer_id) {
    qDebug() << "Поток в ReadBufferChange: " << QThread::currentThread(); //
    qDebug() << "Данные read_buffer в ReadBufferChange: " << read_buffer->data();
    DataHandler *handler = handlers[rr_count_ % max_thread_count_];
    ++rr_count_;

    qRegisterMetaType<const QBuffer*>("const QBuffer*");

    QMetaObject::invokeMethod(handler, "HandleClientRequest",
                Qt::QueuedConnection,
                Q_ARG(const QBuffer*, read_buffer),
                Q_ARG(const quint16, buffer_id));
}

void DataHandlerThreadPool::SetMaxThreadCount(const int thread_count) {
    max_thread_count_ = thread_count;
}

void DataHandlerThreadPool::TransferRequestCreateProfile(const QString table_name, const QString fields, const QString profile_data) {
    emit RequestCreateProfile(table_name, fields, profile_data);
}

void DataHandlerThreadPool::TransferRequestExistsProfile(const QString table_name, const QString fields, const QString auth_data) {
    emit RequestExistsProfile(table_name, fields, auth_data);
}

void DataHandlerThreadPool::TransferSendCompleteAnswer(const QByteArray complete_answer, const int size, const quint16 buffer_id) {
    qDebug() << "id буфера на выходе: " << buffer_id;
    emit SendCompleteAnswer(complete_answer, size, buffer_id);
}

void DataHandlerThreadPool::SendAnswerCreateProfile(const int answer) {
    emit SendAnswerCreateProfileToHandler(answer);
}

void DataHandlerThreadPool::SendAnswerExistsProfile(const int answer) {
    emit SendAnswerExistsProfileToHandler(answer);
}
