#include "QUdpClient.h"

QUdpClient::QUdpClient(QObject *parent) : QObject{parent} {
    socket_ = new QUdpSocket();
}

QUdpClient::~QUdpClient() {
    socket_->close();
    delete socket_;
}

bool QUdpClient::Bind(const QHostAddress address, const quint16 port) {
    connect(socket_, SIGNAL(readyRead()), this, SLOT(HandShakeComplete()));
    int tmp = socket_->bind(address, port);
    if(tmp == true) {
        emit SocketBinded(port);//
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

QString QUdpClient::HandShakeStart() {
    syn_digit_ = QRandomGenerator::global()->bounded(1, 200);
    QString connect_string = QString::number(syn_digit_) +
                             "|" + QString::number(ServerModes::AUTH) +
                             "|" + QString::number(ServerModes::REG) +
                             "|" + QString::number(ServerModes::WORK);
    this->Send(connect_string, QHostAddress::LocalHost, 4352);
    return "OK";
}

bool QUdpClient::AnswerCheck(const QStringList answer_list, const int syn_digit) {
    if(answer_list[0].toInt() == syn_digit + 1
       && answer_list[1].toInt()
       && answer_list[2].toInt()
       && answer_list[3].toInt() == ServerModes::AUTH
       && answer_list[4].toInt() == ServerModes::REG
       && answer_list[5].toInt() == ServerModes::WORK)
        return true;
    else
        return false;
}

QString QUdpClient::HandShakeComplete() {
    QByteArray answer_string;
    answer_string.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(answer_string.data(), answer_string.size());

    QStringList answer_list = QString(answer_string).split("|");
    if(answer_list.size() != 6)
        return "При подключении произошла ошибка";
    if(AnswerCheck(answer_list, syn_digit_)) {
        work_port_ = answer_list[2].toInt();
        QString completion_string = QString::number(answer_list[0].toInt() + 1) + "|"
                                  + QString::number(answer_list[1].toInt() + 1) + "|"
                                  + QString::number(work_port_) + "|"
                                  + QString::number(ServerModes::AUTH) + "|"
                                  + QString::number(ServerModes::REG) + "|"
                                  + QString::number(ServerModes::WORK);
        this->Send(completion_string, server_address_, work_port_);
    }
    disconnect(socket_, SIGNAL(readyRead()), this, SLOT(HandShakeComplete()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
    return "OK";
}
