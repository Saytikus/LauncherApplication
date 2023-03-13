#include "QUdpServer.h"

#include "Interface.h"
#include "ui_Interface.h"

QUdpServer::QUdpServer(QObject *parent) : QObject{parent} {
    socket_ = new QUdpSocket();
}

QUdpServer::~QUdpServer() {
    socket_->close();
    delete socket_;
}

bool QUdpServer::Bind(const QHostAddress address, const quint16 port) {
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
    int tmp = socket_->bind(address, port);
    if(tmp == true)
        return true;
    else
        return false;
}

void QUdpServer::Send(const QString message, const QHostAddress address, const quint16 port) {
    socket_->writeDatagram(message.toUtf8(), address, port);
}

void QUdpServer::Read() {
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size());
    if(!QString(datagram).isEmpty())
        emit ReceivePocket(QString(datagram));
}

void QUdpServer::ReadOne(QHostAddress address, quint16 port) {
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size(), &address, &port);
}

void QUdpServer::Unbind() {
    socket_->close();
}
