#ifndef CLIENTBUFFERPOOL_H
#define CLIENTBUFFERPOOL_H

#include <QObject>
#include "ClientBuffer.h"
#include <QThread> //
#include <QMutex>

class ClientBufferPool : public QObject {
    Q_OBJECT

public:
    explicit ClientBufferPool(QObject *parent = nullptr);

private:
    QVector <ClientBuffer*> *vector_client_buffer_;
    enum Buffers {
        READ,
        SEND
    };
    QMutex mutex;

    void SendToHandler(ClientBuffer* buffer, const QString buffer_id, const int buffer_type);

private slots:
    void CreateClientBuffer(const QHostAddress client_address, const quint16 client_port);

    void WriteReadBuffer(const QByteArray data, const int size, const QString buffer_id);
    void WriteSendBuffer(const QByteArray data, const int size, const QString buffer_id);

    void ClearReadBuffer(const QString buffer_id);
    void ClearSendBuffer(const QString buffer_id);

    void DeleteClientBuffer(const QString buffer_id);
    void QDebugVector(); //
signals:
    void ReadBufferChanged(const QBuffer* read_buffer, const QString buffer_id);
    void SendBufferChanged(const QBuffer* send_buffer, const QString buffer_id);

};

#endif // CLIENTBUFFERPOOL_H
