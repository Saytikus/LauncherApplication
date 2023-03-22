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
    quint16 port_;
    QHostAddress client_address_;
    quint16 client_port_;

    void RedirectMessage(const QString message);

private slots:
    void Read();
    void CompleteConnection();

    void Send(const QString message);

signals:
    void ReceivePocketThread(const QString message);
    void ThreadCreated();

    void ReceiveRegMsg(const QString profile_data);
    void ReceiveAuthMsg(const QString auth_data);
};

#endif // APPTHREAD_H
