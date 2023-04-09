#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QWidget(parent), ui(new Ui::Interface) {
    ui->setupUi(this);
}

Interface::~Interface() {
    delete ui;
}

void Interface::DisplayMessage(const QString datagram) {
    ui->message_win0->addItem(datagram);
}

void Interface::DisplayMessage(const QString datagram, const int count) { //тест
    ui->message_win0->addItem("Сообщение номер: " + datagram);
}

void Interface::on_send_clicked() {
    emit SendClicked(ui->send_message->text(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

void Interface::DisplayAddressPort(const QHostAddress server_address, const quint16 server_port) {
    ui->your_ip0->setText(server_address.toString());
    ui->your_port0->setText(QString::number(server_port));
}

void Interface::on_database_win_call_clicked() {
    emit DataBaseWinCall();
}

