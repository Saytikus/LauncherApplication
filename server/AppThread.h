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
    QUdpSocket *socket_;
    int port_;

private slots:
    void Read();
    void CompleteConnection();

signals:
    void ReceivePocketThread(const QString message);
    void ThreadCreated();
};

#endif // APPTHREAD_H
