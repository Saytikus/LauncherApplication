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

    void RegMsgFromThread(const QString profile_data);
    void AuthMsgFromThread(const QString auth_data);

    void AcceptAnswerInsert(const QString answer);
    void AcceptAnswerExists(const QString answer);
signals:
    void ReceivePocket(const QString datagram);
    void ReceivePocket(const QString datagram, const int count); //тест

    void SocketBinded(const QHostAddress server_address, const quint16 server_port);
    void TransmitRegMsg(const QString message);

    void RequestCreateProfile(const QString table_name, const QString fields, const QString profile_data);

    void RequestExistsProfile(const QString table_name, const QString fields, const QString auth_data);
    void TransmitAuthAnswer(const QString complete_answer);
};

#endif // QUDPSERVER_H
