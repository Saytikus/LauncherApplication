#ifndef CLIENTBUFFER_H
#define CLIENTBUFFER_H

#include <QObject>
#include <QBuffer>
#include <QDebug>
#include <QHostAddress>

class ClientBuffer : public QObject
{
    Q_OBJECT
public:
    explicit ClientBuffer(QHostAddress client_address, quint16 client_port, QObject *parent = nullptr);
    ClientBuffer(const ClientBuffer &other);
    ClientBuffer& operator=(const ClientBuffer &other) { return *this; }

    QString GetAdrPort() { return (client_address_.toString() + QString::number(client_port_)); }

    void WriteReadBuffer(const QByteArray data);
    void WriteSendBuffer(const QByteArray data);

private:
    QHostAddress client_address_;
    quint16 client_port_;

    QBuffer *read_buffer;
    QBuffer *send_buffer;

    float KbSize(const QBuffer &buffer);

private slots:

signals:
    void ReadBufferChanged();
    void SendBufferChanged();
};

#endif // CLIENTBUFFER_H
