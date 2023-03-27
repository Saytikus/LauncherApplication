#include "QUdpServer.h"

QUdpServer::QUdpServer(QObject *parent) : QObject{parent} {
    socket_ = new QUdpSocket();
    qDebug() << "Основной поток: " << QThread::currentThread();
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
        return true;
    }
}

void QUdpServer::Read(){
    QHostAddress sender_address;
    quint16 sender_port;

    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size(), &sender_address, &sender_port);

    QString pair_check = sender_address.toString() + "+" + QString::number(sender_port);
    if(!IpANDPortCheck(pair_check)) {
        if(this->IncomingConnection(datagram, sender_address, sender_port)) {
            addressANDport_vector_.push_back(pair_check);
            pair_check.clear();

            // Позже реализую таблицу БД с подключенными пользователями
            for(int i = 0; i < addressANDport_vector_.size(); i++)
                emit ReceivePocket(QString(addressANDport_vector_[i])); // на экран выводится только первая пара, я это знаю

            ClientBuffer buffer(sender_address, sender_port);
            qDebug() << "Данные порта - " << buffer.GetAdrPort();
            vector_s_buffers.append(buffer);
            vector_s_buffers.size();
        }
    }
    else if(!vector_s_buffers.isEmpty()) {
        for(int i = 0; i < vector_s_buffers.size(); i++) {
            ClientBuffer buf(vector_s_buffers[i]);
            if( buf.GetAdrPort() == sender_address.toString() + QString::number(sender_port) ) {
                buf.WriteReadBuffer(datagram);
            }
        }
    }
    //this->RedirectMessage(QString(datagram));
    QString message = "Данные: " + QString(datagram);
    emit ReceivePocket(message);
    //emit ReceivePocketThread(message);
}

void QUdpServer::Send(const QString message, const QHostAddress address, const quint16 port) {
    socket_->writeDatagram(message.toUtf8(), address, port);
}

void QUdpServer::Unbind() {
    socket_->close();
}

bool QUdpServer::IncomingConnection(const QString message, const QHostAddress sender_address, const quint16 sender_port) {
    QStringList check_list = QString(message).split("|");
    emit ReceivePocket(QString("Пакет из IncomingConnection: " + message));
    if(check_list.size() != 4)
        return false;
    qDebug() << "1 STAD";
    if(ConnectCheck(check_list)) {
        /*int thread_port = QRandomGenerator::global()->bounded(1, 9999);
        AppThread *thread = new AppThread(thread_port);

        connect(thread, SIGNAL(ReceivePocketThread(const QString)), this, SLOT(SendPocket(const QString)), Qt::DirectConnection);
        connect(thread, SIGNAL(ThreadCreated()), this, SLOT(ThreadCountIncrease()), Qt::DirectConnection);

        connect(thread, SIGNAL(ReceiveRegMsg(const QString)), this, SLOT(RegMsgFromThread(const QString)), Qt::DirectConnection);
        connect(this, SIGNAL(TransmitRegAnswer(const QString)), thread, SLOT(Send(const QString)), Qt::DirectConnection);

        connect(thread, SIGNAL(ReceiveAuthMsg(const QString)), this, SLOT(AuthMsgFromThread(const QString)), Qt::DirectConnection);
        connect(this, SIGNAL(TransmitAuthAnswer(const QString)), thread, SLOT(Send(const QString)), Qt::DirectConnection);

        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
        qDebug() << thread_count_; */
        QString answer_string = QString::number(check_list[0].toInt() + 1)
                                + "|" + QString::number(QRandomGenerator::global()->bounded(1, 200))
                                + "|" + QString::number(ServerModes::AUTH)
                                + "|" + QString::number(ServerModes::REG)
                                + "|" + QString::number(ServerModes::WORK);
        this->Send(answer_string, sender_address, sender_port);

        qDebug() << "2 STAD";
        QHostAddress client_address;
        quint16 client_port;

        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);

        //if(sender_address != client_address || sender_port != client_port)
        //    return false;
        qDebug() << "3 STAD";
        // тут будет метод проверки пришедшей строки завершения
        QString message = "Рукопожатие успешно завершено";
        emit ReceivePocket(message);
        //disconnect(socket_, SIGNAL(readyRead()), this, SLOT(CompleteConnection()));
        //connect(socket_, SIGNAL(readyRead()), this, SLOT(Read()), Qt::DirectConnection);
        }
    qDebug() << "4 STAD";
    return true;
}

void QUdpServer::SendCall(const QString message, const QHostAddress address, const quint16 port) {
    this->Send(message, address, port);
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

void QUdpServer::SendPocket(const QString message) {
    emit ReceivePocket(message);
}

void QUdpServer::ThreadCountIncrease() {
    thread_count_++;
}

void QUdpServer::RegMsgFromThread(const QString profile_data) {
    emit RequestCreateProfile("profiles", "login, password", profile_data);
}

void QUdpServer::AuthMsgFromThread(const QString auth_data) {
    emit RequestExistsProfile("profiles", "login, password", auth_data);
}

/*void QUdpServer::AcceptAnswerInsert(const int answer) {
    emit TransmitRegAnswer("reg_answer_start|" + QString::number(answer) + "|reg_answer_end");
}

void QUdpServer::AcceptAnswerExists(const QString answer) {
    emit TransmitAuthAnswer("auth_answer_start|" + answer + "|auth_answer_end");
}*/

/*void QUdpServer::RedirectMessage(const QString message) {
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
}*/
