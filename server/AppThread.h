#ifndef APPTHREAD_H
#define APPTHREAD_H

#include <QObject>
#include <QThread>

#include <QUdpSocket>

class AppThread : public QThread
{
    Q_OBJECT
public:
    explicit AppThread(int id, QObject *parent = nullptr);

    void run();

private:
    int thread_count_ = 1; //тест

    QUdpSocket *socket_;
    int port_;

private slots:
    void Read();

signals:
    void Error(QUdpSocket::SocketError err);
};

#endif // APPTHREAD_H
