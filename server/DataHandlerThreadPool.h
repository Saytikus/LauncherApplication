#ifndef DATAHANDLERTHREADPOOL_H
#define DATAHANDLERTHREADPOOL_H

#include <QObject>
#include <QThread>
#include <QBuffer>
#include "DataHandler.h"

Q_DECLARE_METATYPE(const QBuffer*)

class DataHandlerThreadPool : public QObject {
    Q_OBJECT
public:
    explicit DataHandlerThreadPool(QObject *parent = nullptr);
    void Start();
    void SetMaxThreadCount(const int thread_count);

private:
    int max_thread_count_ = 12;
    int current_thread_count_ = 0;
    int rr_count_ = 0;
    QVector <QThread*> threads;
    QVector <DataHandler*> handlers;

private slots:
    void DeleteHandler(QThread *thread);
    void ReadBufferChange(const QBuffer *read_buffer, const quint16 buffer_id);

    void TransferRequestCreateProfile(const QString table_name, const QString fields, const QString profile_data);
    void TransferRequestExistsProfile(const QString table_name, const QString fields, const QString auth_data);
    void TransferSendCompleteAnswer(const QByteArray complete_answer, const int size, const quint16 buffer_id);

    void SendAnswerCreateProfile(const int answer);
    void SendAnswerExistsProfile(const int answer);
signals:
    void RequestCreateProfile(const QString table_name, const QString fields, const QString profile_data);
    void RequestExistsProfile(const QString table_name, const QString fields, const QString auth_data);
    void SendCompleteAnswer(const QByteArray complete_answer, const int size, const quint16 buffer_id);

    void SendAnswerCreateProfileToHandler(const int answer);
    void SendAnswerExistsProfileToHandler(const int answer);

};

#endif // DATAHANDLERTHREADPOOL_H
