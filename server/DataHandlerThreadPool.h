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
    void ReadBufferChange(const QBuffer *read_buffer);

};

#endif // DATAHANDLERTHREADPOOL_H
