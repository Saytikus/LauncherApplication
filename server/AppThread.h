#ifndef APPTHREAD_H
#define APPTHREAD_H

#include <QObject>
#include <QThread>

#include <QUdpSocket>

class AppThread : public QObject
{
    Q_OBJECT
public:
    explicit AppThread(QObject *parent = nullptr);

private:
    QThread *thread_;
    int thread_count_ = 1; //тест

private slots:
    void MoveToThread(QUdpSocket *socket);
    void Start();

    void IncreaseThreadCount();

signals:

};

#endif // APPTHREAD_H
