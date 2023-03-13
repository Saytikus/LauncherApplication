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

    void ReadStart(QString read_method);

    QUdpSocket *socket_;

private:
    QString datagram_s;

public slots:
    void Read();
    void ReadOne(QHostAddress address, quint16 port);

signals:
    void LetitData(QString datagram);

};

#endif // QUDPSERVER_H
