#include "QUdpClient.h"

QUdpClient::QUdpClient(QObject *parent) : QObject{parent} {
    socket_ = new QUdpSocket();
}

QUdpClient::~QUdpClient() {
    socket_->close();
    delete socket_;
}

bool QUdpClient::Bind(const QHostAddress address, const quint16 port) {
    qDebug() << address.toString();
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

    this->RedirectMessage(QString(datagram));
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
    if(answer_list.size() != 5)
        return "При подключении произошла ошибка";
    if(AnswerCheck(answer_list, syn_digit_)) {
        QString completion_string = QString::number(answer_list[0].toInt() + 1) + "|"
                                  + QString::number(answer_list[1].toInt() + 1) + "|"
                                  + QString::number(ServerModes::AUTH) + "|"
                                  + QString::number(ServerModes::REG) + "|"
                                  + QString::number(ServerModes::WORK);
        this->Send(completion_string, server_address_, work_port_);
    }
    disconnect(socket_, SIGNAL(readyRead()), this, SLOT(HandShakeComplete()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
    return "OK";
}

void QUdpClient::RegRequestSend(const QString login, const QString password) {
    this->Send("reg_start|" + login + "|" + password + "|reg_end" , server_address_, work_port_);
}

void QUdpClient::AuthRequestSend(const QString login, const QString password) {
    this->Send("auth_start|" + login + "|" + password + "|auth_end", server_address_, work_port_);
}

void QUdpClient::RedirectMessage(const QString message) {
    qDebug() << "Пришедшее сообщение: " << message;
    QStringList check_message = message.split("|");

    if(check_message.size() == 3 &&
       check_message[0] == "reg_answer_start" && check_message[2] == "reg_answer_end") {
        emit ReceiveRegAnswer(check_message[1].toInt());
    }

    if(check_message.size() == 3 &&
       check_message[0] == "auth_answer_start" && check_message[2] == "auth_answer_end") {
        emit ReceiveAuthAnswer(check_message[1].toInt());
    }
}

void QUdpClient::SendConnectionClosed() {
    this->Send("request|to close connection", server_address_, work_port_);
}
