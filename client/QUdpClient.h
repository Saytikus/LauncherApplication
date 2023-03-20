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
    quint16 work_port_;
    int syn_digit_;

    bool AnswerCheck(const QStringList answer_list, const int syn_digit);

public slots:

private slots:
    QString Read();
    void SendCall(const QString message,const QHostAddress address, const quint16 port);
    QString HandShakeComplete();

signals:
    void ReceivePocket(const QString message);

    void SocketBinded(const quint16 port); //
};

#endif // QUDPCLIENT_H
