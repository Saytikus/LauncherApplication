#ifndef QUDPCLIENT_H
#define QUDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QRandomGenerator>

class QUdpClient : public QObject
{
    Q_OBJECT
public:
    explicit QUdpClient(QObject *parent = nullptr);
    ~QUdpClient();
    bool Bind(const QHostAddress address, quint16 port);
    void Send(const QString message,const QHostAddress address, const quint16 port);

    //void HandShakeWServer();

    enum ServerModes{
        AUTH,
        REG,
        WORK
    };

private slots:
    QString Read();
    void SendCall(const QString message,const QHostAddress address, const quint16 port);

private:
    QUdpSocket *socket_;

signals:
    void ReceivePocket(const QString message);

    void SocketWasBinded(const quint16 port); //
};

#endif // QUDPCLIENT_H
