#include "QtUDPClient.h"
//#include "ui_widget.h"

QtUDPClient::QtUDPClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtUDPClient)
{
    ui->setupUi(this);
}

QtUDPClient::~QtUDPClient()
{
    delete ui;
}

void QtUDPClient::on_Connect_clicked() {
    socket_ = new QUdpSocket();
    socket_->bind(QHostAddress::LocalHost, ui->bind_port->value());

    ui->Connect->setEnabled(false);
    ui->unbind->setEnabled(true);
    ui->read_addresss->setEnabled(true);
    ui->read_port->setEnabled(true);
    ui->read->setEnabled(true);

    ui->your_port->setText(ui->bind_port->text());
    ui->bind_port->clear();


}

void QtUDPClient::Send(QString message, QHostAddress address, quint16 port) {
    socket_->writeDatagram(message.toLatin1(), address, port);
}

void QtUDPClient::Read() {
    while(socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size());
        ui->listWidget->addItem(QString(datagram));
    }
}

void QtUDPClient::ReadOne() {
    QHostAddress read_address;
    read_address.setAddress(ui->read_addresss->text());
    quint16 read_port = ui->read_port->value();

    while(socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size(), &read_address, &read_port);
        ui->listWidget->addItem(QString(datagram));
    }
}

void QtUDPClient::on_read_clicked() {
    connect(socket_, SIGNAL(readyRead()), this, SLOT(ReadOne()));
}

void QtUDPClient::on_unbind_clicked() {
    socket_->close();
    ui->your_port->clear();
    ui->Connect->setEnabled(true);
    ui->unbind->setEnabled(false);
}

