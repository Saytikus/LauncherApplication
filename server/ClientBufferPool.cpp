#include "ClientBufferPool.h"

ClientBufferPool::ClientBufferPool(QObject *parent) : QObject{parent} {
    vector_client_buffer_ = new QVector <ClientBuffer*>;
}

void ClientBufferPool::CreateClientBuffer(const QHostAddress client_address, const quint16 client_port) {
    ClientBuffer *buffer = new ClientBuffer(client_address, client_port);
    vector_client_buffer_->push_back(buffer);
    qDebug() << "Поток в CreateClientBuffer: " << QThread::currentThread(); //
    qDebug() << "Буфер создан"; //
}

void ClientBufferPool::WriteReadBuffer(const QByteArray data, const int size, const quint16 buffer_id) {
    for(ClientBuffer *buf : *vector_client_buffer_) {
        if(buf->GetBufferId() == buffer_id) {
            buf->WriteBuffer(Buffers::READ, data, size);
            qDebug() << "Записано в буффер чтения: " << QString(data); //
            qDebug() << "Поток в WriteReadBuffer: " << QThread::currentThread(); //
            this->SendToHandler(buf, buffer_id , Buffers::READ);
        }
    }
}

void ClientBufferPool::WriteSendBuffer(const QByteArray data, const int size, const quint16 buffer_id) {
    for(ClientBuffer *buf : *vector_client_buffer_) {
        if(buf->GetBufferId() == buffer_id) {
            buf->WriteBuffer(Buffers::SEND,data, size);
            qDebug() << "Записано в буффер записи: " << QString(data); //
            this->SendToHandler(buf, buffer_id , Buffers::SEND);
        }
    }
}

void ClientBufferPool::SendToHandler(ClientBuffer* buffer, const quint16 buffer_id, const int buffer_type) {
    switch (buffer_type) {
    case Buffers::READ:
        emit ReadBufferChanged(buffer->GetBuffer(Buffers::READ), buffer_id);
        //buffer->ClearBuffer(Buffers::READ); // попытка обосраться
        break;
    case Buffers::SEND:
        emit SendBufferChanged(buffer->GetBuffer(Buffers::SEND), buffer_id);
        //buffer->ClearBuffer(Buffers::SEND); // вторая попытка обосраться
        break;
    }
}
