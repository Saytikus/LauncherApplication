#include "QUdpServer.h"

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

void QUdpServer::Unbind() {
    socket_->close();
}

QString QUdpServer::Read() {
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size());
    if(handshake_successful_ == false && test_count_msg_ == 0 && !QString(datagram).isEmpty()) {
        this->HandShake(QString(datagram));
    }
    else if(!QString(datagram).isEmpty()) {
        emit ReceivePocket(QString(datagram), test_count_msg_);
        test_count_msg_++;
    }
    return QString(datagram);
}

void QUdpServer::BindCall(const QHostAddress address, const quint16 port) {
    this->Bind(address, port);
}

void QUdpServer::UnbindCall() {
    this->Unbind();
}

void QUdpServer::ReadClicked() {
    this->Read();
}

void QUdpServer::SendClicked(const QString message, const QHostAddress address, const quint16 port) {
    this->Send(message, address, port);
}

void QUdpServer::HandShake(QString first_msg) {
    QString connect_string = first_msg;
    emit ReceivePocket("connect_string from client: " + QString(connect_string));
    QStringList check_list = connect_string.split("|");

    for(auto i : check_list)
        emit ReceivePocket("check_list part: " + i);

    bool tmp = check_list[0].toInt() && check_list[1].toInt() == ServerModes::AUTH && check_list[2].toInt() == ServerModes::REG;
    qDebug() << tmp;

    if(check_list[0].toInt() && check_list[1].toInt() == ServerModes::AUTH
                             && check_list[2].toInt() == ServerModes::REG) {
        QString answer_string = QString::number(check_list[0].toInt() + 1)
                                + "|" + QString::number(QRandomGenerator::global()->bounded(1, 200))
                                + "|" + QString::number(ServerModes::AUTH)
                                + "|" + QString::number(ServerModes::REG);
        qDebug() << answer_string;
        this->Send(answer_string, QHostAddress::LocalHost, 7777);
    }

    handshake_successful_ = true;
}
