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

    enum ServerModes{
        AUTH,
        REG,
        WORK
    };

    QString HandShakeStart();

private:
    QUdpSocket *socket_;
    QHostAddress server_address_ = QHostAddress::LocalHost;
    quint16 work_port_ = 4352;
    int syn_digit_;

    bool AnswerCheck(const QStringList answer_list, const int syn_digit);

    void RedirectMessage(const QString message);

public slots:

private slots:
    QString Read();
    void SendCall(const QString message,const QHostAddress address, const quint16 port);
    QString HandShakeComplete();

    void RegRequestSend(const QString login, const QString password);

    void AuthRequestSend(const QString login, const QString password);

signals:
    void ReceivePocket(const QString message);

    void SocketBinded(const quint16 port); //

    void ReceiveRegAnswer(const int reg_answer);
    void ReceiveAuthAnswer(const int auth_answer);
};

#endif // QUDPCLIENT_H
