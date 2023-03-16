#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QStringList>

class QUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit QUdpServer(QObject *parent = nullptr);
    ~QUdpServer();
    bool Bind(const QHostAddress address, const quint16 port);
    void Unbind();
    void Send(const QString message, const QHostAddress address, const quint16 port);
    void HandShake(QString first_msg);

    enum ServerModes{AUTH,
                     REG,
                     WORK};

public slots:
    QString Read();

private:
    QUdpSocket *socket_;
    bool handshake_successful_ = false; //однопоточный позор
    int test_count_msg_ = 0; //однопоточный позор

private slots:
    void BindCall(const QHostAddress address, const quint16 port);
    void UnbindCall();
    void ReadClicked();
    void SendClicked(const QString message, const QHostAddress address, const quint16 port);

signals:
    void ReceivePocket(const QString datagram);
    void ReceivePocket(const QString datagram, const int count); //тест
};

#endif // QUDPSERVER_H
