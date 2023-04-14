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
    QString HandShakeStart();

private:
    QUdpSocket *socket_;
    QHostAddress server_address_ = QHostAddress::LocalHost;
    quint16 work_port_ = 4352;
    int synchronization_digit_;
    enum ServerModes{
        AUTH,
        REG,
        WORK,
        CLOSECONNECTION
    };

    bool AnswerCheck(const QStringList answer_list, const int syn_digit);
    void RedirectMessage(const QString message);

public slots:
    void Send(const QString message,const QHostAddress address, const quint16 port);

private slots:
    QString Read();
    QString HandShakeComplete();

    void RegistrationRequestSend(const QString login, const QString password);
    void AuthorizationRequestSend(const QString login, const QString password);
    void ConnectionClosedSend();

signals:
    void PocketReceived(const QString message);
    void SocketBinded(const quint16 port);

    void RegistrationAnswerReceived(const int reg_answer);
    void AuthorizationAnswerReceived(const int auth_answer);
};

#endif // QUDPCLIENT_H
