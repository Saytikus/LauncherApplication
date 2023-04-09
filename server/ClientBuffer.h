#ifndef CLIENTBUFFER_H
#define CLIENTBUFFER_H

#include <QObject>
#include <QBuffer>
#include <QDebug>
#include <QHostAddress>

class ClientBuffer : public QObject {
    Q_OBJECT
public:
    explicit ClientBuffer(const QHostAddress client_address, const quint16 client_port, QObject *parent = nullptr);
    ClientBuffer(const ClientBuffer &buffer);
    ClientBuffer& operator=(const ClientBuffer &buffer) { return *this; }

    QString GetBufferId() { return buffer_id_; }
    QBuffer* GetBuffer(const int buffer);
    void WriteBuffer(const int buffer, const QByteArray data, const int size);
    void ClearBuffer(const int buffer);

private:
    QString buffer_id_;
    QBuffer *read_buffer_;
    QBuffer *send_buffer_;
    enum Buffers {
        READ,
        SEND
    };

    QString IdComposition(const QHostAddress client_address, const quint16 client_port);
    float KbSize(const QBuffer &buffer);

signals:

};

#endif // CLIENTBUFFER_H
