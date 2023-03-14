#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class QUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit QUdpServer(QObject *parent = nullptr);
    ~QUdpServer();
    bool Bind(const QHostAddress address, const quint16 port);
    void Unbind();
    void Send(const QString message, const QHostAddress address, const quint16 port);

private:
    QUdpSocket *socket_;

public slots:
    void Read();

signals:
    void ReceivePocket(QString datagram);

};

#endif // QUDPSERVER_H
