#include "QUdpServer.h"

QUdpServer::QUdpServer(QObject *parent) : QObject{parent} {
    socket_ = new QUdpSocket();
    qDebug() << "Основной поток: " << QThread::currentThread(); //
}

QUdpServer::~QUdpServer() {
    socket_->close();
    delete socket_;
}

bool QUdpServer::Bind(const QHostAddress address) {
    connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()));
    quint16 port = 4352;
    int tmp = socket_->bind(address, port);
    if(tmp == -1)
        return false;
    else {
        emit SocketBinded(address, port);

        QThread *thread = new QThread();
        thread->setObjectName("Sender thread");
        send_thread = new AppThread();
        send_thread->moveToThread(thread);

        connect(send_thread, SIGNAL(IdDecompositionCall(const QString, const bool)),
                this, SLOT(IdDecomposition(const QString, const bool)), Qt::DirectConnection);
        connect(this, SIGNAL(IdDecompositionFinished(const QStringList)),
                send_thread, SLOT(SendFromBufferContinue(const QStringList)), Qt::DirectConnection);
        connect(send_thread, SIGNAL(SendCall(const QByteArray, const QHostAddress, const quint16)),
                this, SLOT(Send(const QByteArray, const QHostAddress, const quint16)), Qt::DirectConnection);
        connect(send_thread, SIGNAL(HandleSendBufferFinished(const QString)),
                this, SLOT(TransferHandleSendBufferFinished(const QString)), Qt::DirectConnection);

        thread->start();
        return true;
    }
}

void QUdpServer::Read(){
    QHostAddress sender_address;
    quint16 sender_port;

    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size(), &sender_address, &sender_port);

    qDebug() << "Датаграмма: " << datagram;
    QString pair_check = sender_address.toString() + "+" + QString::number(sender_port);
    if(!IpANDPortCheck(pair_check)) {
        if(this->IncomingConnection(datagram, sender_address, sender_port)) {
            addressANDport_vector_.push_back(pair_check);
            pair_check.clear();

            // Позже реализую таблицу БД с подключенными пользователями
            for(int i = 0; i < addressANDport_vector_.size(); i++)
                emit ReceivePocket(QString(addressANDport_vector_[i]));

            emit ConnectionStarted(sender_address, sender_port);
        }
    }
    else if(!addressANDport_vector_.isEmpty())
        emit ReceivePocket(datagram, datagram.size(), this->IdComposition(sender_address, sender_port));
    emit ReceivePocket("Данные: " + QString(datagram));
}

void QUdpServer::Send(const QByteArray message, const QHostAddress address, const quint16 port) {
    qDebug() << "Send start - " << QThread::currentThread();
    qDebug() << "MESSAGE: " << message;
    qDebug() << "ADDR: " << address << " | " << "PORT: " << port;
    socket_->writeDatagram(message, address, port);
}

void QUdpServer::Unbind() {
    socket_->close();
}

bool QUdpServer::IncomingConnection(const QString message, const QHostAddress sender_address, const quint16 sender_port) {
    qDebug() << "Поток в IncomingConnection: " << QThread::currentThread(); //
    QStringList check_list = QString(message).split("|");
    emit ReceivePocket(QString("Пакет из IncomingConnection: " + message));
    if(check_list.size() != 4)
        return false;
    if(ConnectCheck(check_list)) {
        QString answer_string = QString::number(check_list[0].toInt() + 1)
                                + "|" + QString::number(QRandomGenerator::global()->bounded(1, 200))
                                + "|" + QString::number(ServerModes::AUTH)
                                + "|" + QString::number(ServerModes::REG)
                                + "|" + QString::number(ServerModes::WORK);
        this->Send(answer_string.toUtf8(), sender_address, sender_port);

        QHostAddress client_address;
        quint16 client_port;

        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);

        //if(sender_address != client_address || sender_port != client_port)
        //    return false;
        // тут будет метод проверки пришедшей строки завершения
        QString message = "Рукопожатие успешно завершено";
        emit ReceivePocket(message);
        //disconnect(socket_, SIGNAL(readyRead()), this, SLOT(CompleteConnection()));
        //connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()), Qt::DirectConnection);
        }
    return true;
}

void QUdpServer::SendCall(const QString message, const QHostAddress address, const quint16 port) {
    this->Send(message.toUtf8(), address, port);
}

bool QUdpServer::IpANDPortCheck(const QString pair) {
    if(addressANDport_vector_.indexOf(pair) == -1)
        return false;
    else
        return true;
}

bool QUdpServer::ConnectCheck(QStringList check_list) {
    if(check_list[0].toInt() && check_list[1].toInt() == ServerModes::AUTH
                             && check_list[2].toInt() == ServerModes::REG
                             && check_list[3].toInt() == ServerModes::WORK)
        return true;
    else
        return false;
}

QString QUdpServer::IdComposition(const QHostAddress client_address, const quint16 client_port) {
    return (client_address.toString() + "_" + QString::number(client_port));
}

QStringList QUdpServer::IdDecomposition(const QString buffer_id, const bool need_signal) {
    qDebug() << "IdDecomposition start";
    qDebug() << QThread::currentThread();
    if(need_signal)
        emit IdDecompositionFinished(buffer_id.split("_"));
    return buffer_id.split("_");
}

void QUdpServer::SendPocket(const QString message) {
    emit ReceivePocket(message);
}

void QUdpServer::SendBufferChange(const QBuffer* send_buffer, const QString buffer_id) {
    qDebug() << "Поток в SendBufferChange: " << QThread::currentThread();

    QMetaObject::invokeMethod(send_thread, "SendFromBufferStart",
                              Qt::QueuedConnection,
                              Q_ARG(const QBuffer*, send_buffer),
                              Q_ARG(const QString, buffer_id));
}

void QUdpServer::TransferHandleSendBufferFinished(const QString buffer_id) {
    emit HandleSendBufferFinished(buffer_id);
}
