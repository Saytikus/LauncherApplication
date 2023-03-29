#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QStringList>

#include "AppThread.h"
#include <QVector>
#include "ClientBuffer.h"

class QUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit QUdpServer(QObject *parent = nullptr);
    ~QUdpServer();
    bool Bind(const QHostAddress address);
    void Unbind();
    enum ServerModes{AUTH,
                     REG,
                     WORK};

private:
    QUdpSocket *socket_;
    int thread_count_ = 2;

    QVector<QString> addressANDport_vector_; //пока что вместо БД
    bool IpANDPortCheck(const QString pair); //пока что вместо БД

    bool ConnectCheck(QStringList check_list);
    quint16 IdFormation(const QHostAddress client_address, const quint16 client_port);

private slots:
    void Read();
    void Send(const QString message, const QHostAddress address, const quint16 port);

    void SendCall(const QString message, const QHostAddress address, const quint16 port);
    bool IncomingConnection(const QString message, const QHostAddress sender_address, const quint16 sender_port);
    void SendPocket(const QString message);
    void ThreadCountIncrease();

signals:
    void ReceivePocket(const QString datagram);
    void ReceivePocket(const QString datagram, const int count); //тест
    void ReceivePocket(const QByteArray data, const int size, const quint16 buffer_id);

    void SocketBinded(const QHostAddress server_address, const quint16 server_port);

    void ConnectionStarted(const QHostAddress client_address, const quint16 client_port);
};

#endif // QUDPSERVER_H
