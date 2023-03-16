#ifndef QUDPCLIENT_H
#define QUDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class QUdpClient : public QObject
{
    Q_OBJECT
public:
    explicit QUdpClient(QObject *parent = nullptr);
    ~QUdpClient();
    bool Bind(const QHostAddress address, quint16 port);
    void Send(const QString message,const QHostAddress address, const quint16 port);

private slots:
    void Read();
    void SendCall(const QString message,const QHostAddress address, const quint16 port);

private:
    QUdpSocket *socket_;

signals:
    void ReceivePocket(const QString message);
};

#endif // QUDPCLIENT_H
