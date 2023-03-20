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

    QString message = "Данные: " + QString(datagram);
    emit ReceivePocketThread(message);
}

void AppThread::CompleteConnection() {
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size());

    QString message = "Рукопожатие успешно завершено";
    emit ReceivePocketThread(message);
    qDebug() << QThread::currentThread();
    disconnect(socket_, SIGNAL(readyRead()), this, SLOT(CompleteConnection()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()), Qt::DirectConnection);
}
