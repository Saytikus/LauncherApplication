#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QWidget(parent), ui(new Ui::Interface) {
    ui->setupUi(this);
}

Interface::~Interface() {
    delete ui;
}


void Interface::on_bind0_clicked() {
    server = new QUdpServer();
    connect(server, SIGNAL(ReceivePocket(QString)), this, SLOT(DisplayMessage(QString)));
    server->Bind(QHostAddress::LocalHost, ui->bind_port0->value());


    ui->bind0->setEnabled(false);
    ui->unbind0->setEnabled(true);
    ui->read_ip0->setEnabled(true);
    ui->read_port0->setEnabled(true);
    ui->read_one0->setEnabled(true);

    ui->your_port0->setText(ui->bind_port0->text());
    ui->bind_port0->clear();
}


void Interface::on_unbind0_clicked() {
    server->Unbind();

    ui->your_port0->clear();
    ui->bind0->setEnabled(true);
    ui->unbind0->setEnabled(false);
}


void Interface::on_read_one0_clicked() {

}


void Interface::on_read0_clicked() {
    server->Read();

    ui->read0->setEnabled(false);
}

void Interface::DisplayMessage(QString datagram) {
    datagram_ = datagram;
    ui->message_win0->addItem(datagram_);
}
