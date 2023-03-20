#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QStringList>

#include "AppThread.h"
#include <QVector>

class QUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit QUdpServer(QObject *parent = nullptr);
    ~QUdpServer();
    bool Bind(const QHostAddress address);
    void Unbind();
    void Send(const QString message, const QHostAddress address, const quint16 port);

    enum ServerModes{AUTH,
                     REG,
                     WORK};

public slots:

private:
    QUdpSocket *socket_;
    int thread_count_ = 2;

    QVector<QString> addressANDport_vector_; //пока что вместо БД
    bool IpANDPortCheck(const QString pair); //пока что вместо БД

    bool ConnectCheck(QStringList check_list);

private slots:
    void SendCall(const QString message, const QHostAddress address, const quint16 port);
    QString IncomingConnection();
    void SendPocket(const QString message);
    void ThreadCountIncrease();

signals:
    void ReceivePocket(const QString datagram);
    void ReceivePocket(const QString datagram, const int count); //тест

    void SocketBinded(const QHostAddress server_address, const quint16 server_port);

};

#endif // QUDPSERVER_H
