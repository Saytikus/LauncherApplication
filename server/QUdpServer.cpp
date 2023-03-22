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
    connect(socket_, SIGNAL(readyRead()), this, SLOT(IncomingConnection()));
    quint16 port = 4352;
    int tmp = socket_->bind(address, port);
    if(tmp == -1)
        return false;
    else {
        emit SocketBinded(address, port);
        return true;
    }
}

void QUdpServer::Send(const QString message, const QHostAddress address, const quint16 port) {
    socket_->writeDatagram(message.toUtf8(), address, port);
}

void QUdpServer::Unbind() {
    socket_->close();
}

QString QUdpServer::IncomingConnection() {
    QHostAddress sender_address;
    quint16 sender_port;

    QByteArray datagram;
    datagram.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(datagram.data(), datagram.size(), &sender_address, &sender_port);

    QStringList check_list = QString(datagram).split("|");
    if(check_list.size() != 4)
        return "Не подлюченный пользователь";
    if(ConnectCheck(check_list)) {
        QString pair_check = sender_address.toString() + "+" + QString::number(sender_port);

        if(!IpANDPortCheck(pair_check)) {
            addressANDport_vector_.push_back(pair_check);
            pair_check.clear();

            emit ReceivePocket(QString(addressANDport_vector_[0])); // на экран выводится только первая пара, я это знаю

            int thread_port = QRandomGenerator::global()->bounded(1, 9999);
            AppThread *thread = new AppThread(thread_port);

            connect(thread, SIGNAL(ReceivePocketThread(const QString)), this, SLOT(SendPocket(const QString)), Qt::DirectConnection);
            connect(thread, SIGNAL(ThreadCreated()), this, SLOT(ThreadCountIncrease()), Qt::DirectConnection);
            connect(thread, SIGNAL(ReceiveRegMsg(const QString)), this, SLOT(RegMsgFromThread(const QString)), Qt::DirectConnection);
            connect(thread, SIGNAL(ReceiveAuthMsg(const QString)), this, SLOT(AuthMsgFromThread(const QString)), Qt::DirectConnection);
            connect(this, SIGNAL(TransmitAuthAnswer(const QString)), thread, SLOT(Send(const QString)), Qt::DirectConnection);

            thread->start();
            qDebug() << thread_count_;
            QString answer_string = QString::number(check_list[0].toInt() + 1)
                                    + "|" + QString::number(QRandomGenerator::global()->bounded(1, 200))
                                    + "|" + QString::number(thread_port)
                                    + "|" + QString::number(ServerModes::AUTH)
                                    + "|" + QString::number(ServerModes::REG)
                                    + "|" + QString::number(ServerModes::WORK);
            this->Send(answer_string, sender_address, sender_port);
            // здесь должен создаваться поток и продолжаться хендшейк
        }
    }

    emit ReceivePocket(QString("Пакет из IncomingConnection: " + datagram));
    return "OK";
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

void QUdpServer::AcceptAnswerInsert(const QString answer) {

}

void QUdpServer::AcceptAnswerExists(const QString answer) {
    emit TransmitAuthAnswer("auth_answer_start|" + answer + "|auth_answer_end");
}
