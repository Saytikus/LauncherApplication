#include "ClientBufferPool.h"

ClientBufferPool::ClientBufferPool(QObject *parent) : QObject{parent} {
    client_buffers_ = new QVector <ClientBuffer*>;
}

void ClientBufferPool::CreateClientBuffer(const QHostAddress client_address, const quint16 client_port) {
    ClientBuffer *buffer = new ClientBuffer(client_address, client_port);
    client_buffers_->push_back(buffer);
    qDebug() << "Поток в CreateClientBuffer: " << QThread::currentThread(); //
    qDebug() << "Буфер создан"; //
}

void ClientBufferPool::WriteReadBuffer(const QByteArray data, const int size, const QString buffer_id) {
    for(ClientBuffer *buf : *client_buffers_) {
        if(buf->GetBufferId() == buffer_id) {           
            buf->WriteBuffer(Buffers::READ, data, size);
            qDebug() << "Записано в буффер чтения: " << QString(data); //
            qDebug() << "Поток в WriteReadBuffer: " << QThread::currentThread(); //
            this->SendToHandler(buf, buffer_id , Buffers::READ); 
        }
    }
}

void ClientBufferPool::WriteSendBuffer(const QByteArray data, const int size, const QString buffer_id) {
    for(ClientBuffer *buf : *client_buffers_) {
        if(buf->GetBufferId() == buffer_id) {
            mutex.lock();
            buf->WriteBuffer(Buffers::SEND,data, size);
            qDebug() << "Записано в буффер записи: " << QString(data); //
            this->SendToHandler(buf, buffer_id , Buffers::SEND);
            mutex.unlock();
        }
    }
}

void ClientBufferPool::SendToHandler(ClientBuffer* buffer, const QString buffer_id, const int buffer_type) {
    switch (buffer_type) {
    case Buffers::READ:
        emit ReadBufferChanged(buffer->GetBuffer(Buffers::READ), buffer_id);
        break;
    case Buffers::SEND:
        emit SendBufferChanged(buffer->GetBuffer(Buffers::SEND), buffer_id);
        break;
    }
}

void ClientBufferPool::ClearReadBuffer(const QString buffer_id) {
    for(ClientBuffer *buf : *client_buffers_) {
        if(buf->GetBufferId() == buffer_id) {
            mutex.lock();
            qDebug() << "Буфер чтения до очищения - " << buf->GetBuffer(Buffers::READ)->buffer().data();
            buf->ClearBuffer(Buffers::READ);
            qDebug() << "Буфер чтения после очищения - " << buf->GetBuffer(Buffers::READ)->buffer().data();
            mutex.unlock();
        }
    }
}

void ClientBufferPool::ClearSendBuffer(const QString buffer_id) {
    for(ClientBuffer *buf : *client_buffers_) {
        if(buf->GetBufferId() == buffer_id) {
            mutex.lock();
            qDebug() << "Буфер записи до очищения - " << buf->GetBuffer(Buffers::SEND)->buffer().data();
            buf->ClearBuffer(Buffers::SEND);
            qDebug() << "Буфер записи после очищения - " << buf->GetBuffer(Buffers::SEND)->buffer().data();
            mutex.unlock();
        }
    }
}

void ClientBufferPool::DeleteClientBuffer(const QString buffer_id) {
    for(ClientBuffer *buf : *client_buffers_) {
        if(buf->GetBufferId() == buffer_id) {
            mutex.lock();
            qDebug() << client_buffers_->removeOne(buf);
            mutex.unlock();
        }
    }
}
