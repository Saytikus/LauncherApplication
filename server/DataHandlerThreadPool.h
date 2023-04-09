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
    void ReadBufferChange(const QBuffer *read_buffer, const QString buffer_id);

    void TransferRequestCreateProfile(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id);
    void TransferRequestExistsProfile(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id);
    void TransferSendCompleteAnswer(const QByteArray complete_answer, const int size, const QString buffer_id);
    void TransferHandleRequestFinished(const QString buffer_id);
    void TransferRequestDeleteBuffer(const QString buffer_id);

    void SendAnswerCreateProfile(const int answer, const QString buffer_id);
    void SendAnswerExistsProfile(const int answer, const QString buffer_id);
signals:
    void RequestCreateProfile(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id);
    void RequestExistsProfile(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id);
    void FormCompleteAnswerFinished(const QByteArray complete_answer, const int size, const QString buffer_id);
    void HandleReadBufferFinished(const QString buffer_id);
    void RequestDeleteBuffer(const QString buffer_id);
};

#endif // DATAHANDLERTHREADPOOL_H
