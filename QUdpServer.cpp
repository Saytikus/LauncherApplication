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
    Interface interface;
    connect(this, SIGNAL(LetitData(QString)), &interface, SLOT(Disp(QString)));
    while(socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size());
        datagram_s = "start" + QString(datagram) + "end";
        emit LetitData(datagram_s);
    }
    //ui->message_win->addItem(QString(datagram));
}

void QUdpServer::ReadOne(QHostAddress address, quint16 port) {
    while(socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size(), &address, &port);
    }
}

void QUdpServer::Unbind() {
    socket_->close();
}

void QUdpServer::ReadStart(QString read_method) {
    //if(read_method == "Read" or read_method == "Read()")
        //connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
    //if(read_method == "ReadOne" or read_method == "ReadOne()")
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
}

