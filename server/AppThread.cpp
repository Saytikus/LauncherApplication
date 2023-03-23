#include "AppThread.h"

AppThread::AppThread(int id, QObject *parent) : QThread{parent} {
    port_ = id;
}

void AppThread::run() {
    qDebug() << "Подрубил потокич";
    socket_ = new QUdpSocket();

    connect(socket_, SIGNAL(readyRead()), this, SLOT(CompleteConnection()), Qt::DirectConnection);

    socket_->bind(QHostAddress::LocalHost, port_);
    emit ThreadCreated();

    qDebug() << "Порт для работы с клиентом(куда он теперь должен отправлять данные): " << port_;
    exec();
}

void AppThread::Read(){
    setlocale(LC_ALL, "ru");
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size());

    this->RedirectMessage(QString(datagram));
    QString message = "Данные: " + QString(datagram);
    emit ReceivePocketThread(message);
}

void AppThread::CompleteConnection() { 
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size(), &client_address_, &client_port_);

    // тут будет метод проверки пришедшей строки завершения
    QString message = "Рукопожатие успешно завершено";
    emit ReceivePocketThread(message);
    qDebug() << QThread::currentThread();
    disconnect(socket_, SIGNAL(readyRead()), this, SLOT(CompleteConnection()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()), Qt::DirectConnection);
}

void AppThread::RedirectMessage(const QString message) {
    QStringList check_message = message.split("|");
    if(check_message.size() == 4 &&
       check_message[0] == "reg_start" && check_message[3] == "reg_end") {
        QString profile_data = check_message[1] + "!" + check_message[2];
        qDebug() << "profile_data from thread: " << profile_data;
        emit ReceiveRegMsg(profile_data);
    }

    if(check_message.size() == 4 &&
       check_message[0] == "auth_start" && check_message[3] == "auth_end") {
        QString auth_data = check_message[1] + "!" + check_message[2];
        qDebug() << auth_data;
        emit ReceiveAuthMsg(auth_data);
    }
}

void AppThread::Send(const QString message) {
    socket_->writeDatagram(message.toUtf8(), client_address_, client_port_);
}
