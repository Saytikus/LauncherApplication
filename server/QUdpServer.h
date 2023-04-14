#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QStringList>

#include "Sender.h"
#include <QVector>

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
    Sender *sender;

    QVector<QString> addressANDport_vector_; //пока что вместо БД
    bool IpANDPortCheck(const QString pair); //пока что вместо БД

    bool ConnectCheck(QStringList check_list);
    QString IdComposition(const QHostAddress client_address, const quint16 client_port);

private slots:
    void Read();
    void Send(const QByteArray message, const QHostAddress address, const quint16 port);

    void SendCall(const QString message, const QHostAddress address, const quint16 port);
    bool IncomingConnection(const QString message, const QHostAddress sender_address, const quint16 sender_port);
    void SendPocket(const QString message);
    void SendBufferChange(const QBuffer* send_buffer, const QString buffer_id);
    void TransferHandleSendBufferFinished(const QString buffer_id);

    QStringList IdDecomposition(const QString buffer_id, const bool need_signal = false);

signals:
    void ReceivePocket(const QString datagram);
    void ReceivePocket(const QString datagram, const int count); //тест
    void ReceivePocket(const QByteArray data, const int size, const QString buffer_id);

    void SocketBinded(const QHostAddress server_address, const quint16 server_port);

    void ConnectionStarted(const QHostAddress client_address, const quint16 client_port);

    void IdDecompositionFinished(const QStringList address_port_list);

    void HandleSendBufferFinished(const QString buffer_id);
};

#endif // QUDPSERVER_H
