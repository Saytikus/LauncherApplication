#include "QUdpClient.h"
#include "ui_QUdpClient.h"

QUdpClient::QUdpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QUdpClient)
{
    ui->setupUi(this);
    socket_ = new QUdpSocket();
    socket_->bind(QHostAddress::LocalHost, 7777);
}

QUdpClient::~QUdpClient()
{
    delete ui;
}

bool QUdpClient::Bind(const QHostAddress address, const quint16 port) {
    int tmp = socket_->bind(address, port);
    if(tmp == true)
        return true;
    else
        return false;
}

void QUdpClient::Send(QString message, quint16 port) {
    socket_->writeDatagram(message.toUtf8(), QHostAddress::LocalHost, port);
}

void QUdpClient::on_send_clicked() {
    //QUdpClient client;
    //client.Send(ui->send_message->text(), ui->send_port->value());
    socket_->writeDatagram(ui->send_message->text().toUtf8(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

