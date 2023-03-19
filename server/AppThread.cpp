#include "AppThread.h"

AppThread::AppThread(int id, QObject *parent) : QThread{parent} {
    port_ = id;
}

void AppThread::run() {
    qDebug() << "Подрубил потокич";
    socket_ = new QUdpSocket();

    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()), Qt::DirectConnection);

    socket_->bind(QHostAddress::LocalHost, port_);

    qDebug() << "Порт для работы с клиентом(куда он теперь должен отправлять данные): " << port_;
    exec();
}

void AppThread::Read(){
    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size());
    qDebug() << "Данные: " << datagram << " Поток: " << QThread::currentThread();
}
