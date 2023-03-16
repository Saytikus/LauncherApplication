#include "Interface.h"
#include "ui_Interface.h"

Interface::Interface(QWidget *parent) : QWidget(parent), ui(new Ui::Interface) {
    ui->setupUi(this);
}

Interface::~Interface() {
    delete ui;
}


void Interface::on_bind0_clicked() {
    emit BindClicked(QHostAddress::LocalHost, ui->bind_port0->value());

    ui->bind0->setEnabled(false);
    ui->unbind0->setEnabled(true);
    ui->read_ip0->setEnabled(true);
    ui->read_port0->setEnabled(true);
    ui->read_one0->setEnabled(true);

    ui->your_port0->setText(ui->bind_port0->text());
    ui->bind_port0->clear();
}


void Interface::on_unbind0_clicked() {
    emit UnbindClicked();

    ui->your_port0->clear();
    ui->bind0->setEnabled(true);
    ui->unbind0->setEnabled(false);
    ui->read0->setEnabled(true);
}


//void Interface::on_read_one0_clicked() {} заморожено до лучших времён


void Interface::on_read0_clicked() {
    emit ReadClicked();

    ui->read0->setEnabled(false);

    ui->read_mode_info->setText("Вы принимаете сообщения.");
}

void Interface::DisplayMessage(const QString datagram) {
    ui->message_win0->addItem(datagram);
}

void Interface::DisplayMessage(const QString datagram, const int count) { //тест
    ui->message_win0->addItem("Сообщение номер: " + datagram);
}

void Interface::on_send_clicked() {
    emit SendClicked(ui->send_message->text().toUtf8(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

void Interface::TestDisplay(QString data) {
    ui->message_win0->addItem("TestDisplay:" + data);
}
