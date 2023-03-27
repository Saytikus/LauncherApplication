#include "ClientBuffer.h"

ClientBuffer::ClientBuffer(QHostAddress client_address, quint16 client_port, QObject *parent) : QObject{parent} {
    client_address_ = client_address;
    client_port_ = client_port;
    read_buffer = new QBuffer();
    send_buffer = new QBuffer();

    qDebug() << client_address_;
    qDebug() << client_port_;
}

ClientBuffer::ClientBuffer(const ClientBuffer &other) {
    client_address_ = other.client_address_;
    client_port_ = other.client_port_;

    read_buffer = other.read_buffer;
    send_buffer = other.send_buffer;
}

float ClientBuffer::KbSize(const QBuffer &buffer) {
    float kb_size = 0;
    kb_size = buffer.size();
    kb_size /= 1000;
    qDebug() << "size of buffer in kb = " << kb_size;
    return kb_size;
}

void ClientBuffer::WriteReadBuffer(const QByteArray data) {
    read_buffer->open(QIODevice::WriteOnly);
    read_buffer->write(data);
    qDebug() << "Данные из буфера чтения - " << read_buffer->data();
    qDebug() << "Еще мета - " << read_buffer->bytesToWrite();
    read_buffer->close();
}

void ClientBuffer::WriteSendBuffer(const QByteArray data) {
    send_buffer->open(QIODevice::WriteOnly);
    send_buffer->write(data);
    send_buffer->close();
}
