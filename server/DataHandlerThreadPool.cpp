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
        qDebug() << handler->thread();
        //connect(thread, SIGNAL(finished), handler, SLOT(deleteLater));
        connect(handler, SIGNAL(HandleClientRequestFinished(const QString)),
                this, SLOT(TransferHandleRequestFinished(const QString)), Qt::DirectConnection);
        connect(handler, SIGNAL(RequestCreateProfileToPool(const QString, const QString, const QString, const QString)),
                this, SLOT(TransferRequestCreateProfile(const QString, const QString, const QString, const QString)), Qt::DirectConnection);
        connect(handler, SIGNAL(RequestExistsProfileToPool(const QString, const QString, const QString, const QString)),
                this, SLOT(TransferRequestExistsProfile(const QString, const QString, const QString, const QString)), Qt::DirectConnection);
        connect(handler, SIGNAL(SendCompleteAnswerToPool(const QByteArray, const int, const QString)),
                this, SLOT(TransferSendCompleteAnswer(const QByteArray, const int, const QString)), Qt::DirectConnection);
        connect(handler, SIGNAL(RequestDeleteBufferToPool(const QString)),
                this, SLOT(TransferRequestDeleteBuffer(const QString)), Qt::DirectConnection);

        threads.push_back(thread);
        handlers.push_back(handler);

        thread->start();
    }
}

void DataHandlerThreadPool::DeleteHandler(QThread *thread) {
    thread->quit();
    thread->wait();
}

void DataHandlerThreadPool::ReadBufferChange(const QBuffer *read_buffer, const QString buffer_id) {
    qDebug() << "Поток в ReadBufferChange: " << QThread::currentThread(); //
    qDebug() << "Данные read_buffer в ReadBufferChange: " << read_buffer->data();
    DataHandler *handler = handlers[rr_count_ % max_thread_count_];
    ++rr_count_;

    qRegisterMetaType<const QBuffer*>("const QBuffer*");

    QMetaObject::invokeMethod(handler, "HandleClientRequest",
                Qt::QueuedConnection,
                Q_ARG(const QBuffer*, read_buffer),
                Q_ARG(const QString, buffer_id));
}

void DataHandlerThreadPool::SetMaxThreadCount(const int thread_count) {
    max_thread_count_ = thread_count;
}

void DataHandlerThreadPool::TransferRequestCreateProfile(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id) {
    emit RequestCreateProfile(table_name, fields, profile_data, buffer_id);
}

void DataHandlerThreadPool::TransferRequestExistsProfile(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id) {
    emit RequestExistsProfile(table_name, fields, auth_data, buffer_id);
}

void DataHandlerThreadPool::TransferSendCompleteAnswer(const QByteArray complete_answer, const int size, const QString buffer_id) {
    qDebug() << "id буфера на выходе: " << buffer_id;
    emit FormCompleteAnswerFinished(complete_answer, size, buffer_id);
}

void DataHandlerThreadPool::TransferHandleRequestFinished(const QString buffer_id) {
    emit HandleReadBufferFinished(buffer_id);
}

void DataHandlerThreadPool::SendAnswerCreateProfile(const int answer, const QString buffer_id) {
    qDebug() << "Поток в SendAnswerCreateProfile: " << QThread::currentThread(); //
    qDebug() << "Данные в SendAnswerCreateProfile: " << QString::number(answer);
    DataHandler *handler = handlers[rr_count_ % max_thread_count_];
    ++rr_count_;

    QMetaObject::invokeMethod(handler, "HandleAnswerCreateProfile",
                Qt::QueuedConnection,
                Q_ARG(const int, answer),
                Q_ARG(const QString, buffer_id));
}

void DataHandlerThreadPool::SendAnswerExistsProfile(const int answer, const QString buffer_id) {
    qDebug() << "Поток в SendAnswerExistsProfile: " << QThread::currentThread(); //
    qDebug() << "Данные в SendAnswerExistsProfile: " << QString::number(answer);
    DataHandler *handler = handlers[rr_count_ % max_thread_count_];
    ++rr_count_;

    QMetaObject::invokeMethod(handler, "HandleAnswerExistsProfile",
                Qt::QueuedConnection,
                Q_ARG(const int, answer),
                Q_ARG(const QString, buffer_id));
}

void DataHandlerThreadPool::TransferRequestDeleteBuffer(const QString buffer_id) {
    emit RequestDeleteBuffer(buffer_id);
}
