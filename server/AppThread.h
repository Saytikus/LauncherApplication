#ifndef APPTHREAD_H
#define APPTHREAD_H

#include <QObject>
#include <QThread>
#include <QHostAddress>

class AppThread : public QObject
{
    Q_OBJECT
public:
    explicit AppThread(QObject *parent = nullptr);

private:

    //void RedirectMessage(const QString message);

    //void Disconnect();
private slots:
    //void Read();
    //void CompleteConnection();

    //void Send(const QString message);

signals:
    void SendCall(const QString message, const QHostAddress address, const quint16 port);
    //void ReceivePocketThread(const QString message);
    //void ThreadCreated();

    //void ReceiveRegMsg(const QString profile_data);
    //void ReceiveAuthMsg(const QString auth_data);
};

#endif // APPTHREAD_H
