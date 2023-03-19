#include "QUdpClient.h"

QUdpClient::QUdpClient(QObject *parent) : QObject{parent} {
    socket_ = new QUdpSocket();
}

QUdpClient::~QUdpClient() {
    socket_->close();
    delete socket_;
}

bool QUdpClient::Bind(const QHostAddress address, const quint16 port) {
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
    int tmp = socket_->bind(address, port);
    if(tmp == true) {
        emit SocketWasBinded(port);//
        this->HandShakeWServer();
        return true;
    }
    else
        return false;
}

void QUdpClient::Send(QString message, const QHostAddress address, quint16 port) {
    socket_->writeDatagram(message.toUtf8(), address, port);
}

QString QUdpClient::Read() {
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size());
    if(!QString(datagram).isEmpty())
        emit ReceivePocket(QString(datagram));
    return QString(datagram);
}

void QUdpClient::SendCall(const QString message, const QHostAddress address, const quint16 port) {
    this->Send(message, address, port);
}

void QUdpClient::HandShakeWServer() {
    QString connect_string = QString::number(QRandomGenerator::global()->bounded(1, 200)) +
            "|" + QString::number(ServerModes::AUTH) +
            "|" + QString::number(ServerModes::REG);
    this->Send(connect_string, QHostAddress::LocalHost, 2222);
    emit ReceivePocket("connect_string send to server: " + connect_string);

}
