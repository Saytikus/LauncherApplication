#ifndef APPTHREAD_H
#define APPTHREAD_H

#include <QObject>
#include <QThread>
#include <QHostAddress>
#include <QBuffer>

class AppThread : public QObject
{
    Q_OBJECT
public:
    explicit AppThread(QObject *parent = nullptr);

    Q_INVOKABLE void SendFromBufferStart(const QBuffer* buffer, const QString buffer_id);

private:
    QByteArray send_data_;
    //void RedirectMessage(const QString message);
    void SendFromBufferComplete(const QByteArray send_data, const QHostAddress client_address, const quint16 client_port);
    //void Disconnect();
private slots:
    void SendFromBufferContinue(const QStringList address_port_list);

    //void Read();
    //void CompleteConnection();

    //void Send(const QString message);

signals:
    void IdDecompositionCall(const QString buffer_id, const bool need_signal);
    void SendCall(const QByteArray message, const QHostAddress address, const quint16 port);
    void HandleSendBufferFinished(const QString buffer_id);
    //void ReceivePocketThread(const QString message);
    //void ThreadCreated();

    //void ReceiveRegMsg(const QString profile_data);
    //void ReceiveAuthMsg(const QString auth_data);
};

#endif // APPTHREAD_H
