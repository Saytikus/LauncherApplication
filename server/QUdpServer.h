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

public slots:

private:
    QUdpSocket *socket_;
    int thread_count_ = 2;

    QVector<QString> addressANDport_vector_; //пока что вместо БД
    bool IpANDPortCheck(const QString pair); //пока что вместо БД

    bool ConnectCheck(QStringList check_list);

    QVector <ClientBuffer> vector_s_buffers;

private slots:
    void Read();
    void Send(const QString message, const QHostAddress address, const quint16 port);

    void SendCall(const QString message, const QHostAddress address, const quint16 port);
    bool IncomingConnection(const QString message, const QHostAddress sender_address, const quint16 sender_port);
    void SendPocket(const QString message);
    void ThreadCountIncrease();

    void RegMsgFromThread(const QString profile_data);
    void AuthMsgFromThread(const QString auth_data);

    //void AcceptAnswerInsert(const int answer);
    //void AcceptAnswerExists(const QString answer);
signals:
    void ReceivePocket(const QString datagram);
    void ReceivePocket(const QString datagram, const int count); //тест

    void SocketBinded(const QHostAddress server_address, const quint16 server_port);

    void RequestCreateProfile(const QString table_name, const QString fields, const QString profile_data);
    //void TransmitRegAnswer(const QString complete_answer);

    void RequestExistsProfile(const QString table_name, const QString fields, const QString auth_data);
    //void TransmitAuthAnswer(const QString complete_answer);
};

#endif // QUDPSERVER_H
