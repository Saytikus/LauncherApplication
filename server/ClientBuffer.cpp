#include "ClientBuffer.h"

ClientBuffer::ClientBuffer(const QHostAddress client_address, const quint16 client_port, QObject *parent) : QObject{parent} {
    buffer_id_ = IdComposition(client_address, client_port);

    read_buffer_ = new QBuffer();
    send_buffer_ = new QBuffer();
}

ClientBuffer::ClientBuffer(const ClientBuffer &buffer) {
    buffer_id_ = buffer.buffer_id_;
    read_buffer_ = buffer.read_buffer_;
    send_buffer_ = buffer.send_buffer_;
}

QString ClientBuffer::IdComposition(const QHostAddress client_address, const quint16 client_port) {
    return (client_address.toString() + "_" + QString::number(client_port));
}

float ClientBuffer::KbSize(const QBuffer &buffer) {
    float kb_size = 0;
    kb_size = buffer.size();
    kb_size /= 1000;
    qDebug() << "size of buffer in kb = " << kb_size;
    return kb_size;
}

QBuffer* ClientBuffer::GetBuffer(const int buffer) {
    switch (buffer) {
    case Buffers::READ:
        return read_buffer_;
        break;
    case Buffers::SEND:
        return send_buffer_;
        break;
    }
}

void ClientBuffer::WriteBuffer(const int buffer, const QByteArray data, const int size) {
    switch (buffer) {
    case Buffers::READ:
        read_buffer_->open(QIODevice::WriteOnly);
        read_buffer_->write(data, size);
        qDebug() << "Данные из буфера чтения - " << read_buffer_->data();
        read_buffer_->close();
        break;
    case Buffers::SEND:
        send_buffer_->open(QIODevice::WriteOnly);
        send_buffer_->write(data, size);
        send_buffer_->close();
        break;
    }

}
void ClientBuffer::ClearBuffer(const int buffer) {
    switch (buffer) {
    case Buffers::READ:
        read_buffer_->buffer().clear();
        break;
    case Buffers::SEND:
        send_buffer_->buffer().clear();
        break;
    }

}
