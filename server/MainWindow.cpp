#include "MainWindow.h"
#include "ui_Interface.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::DisplayMessageOnMessageWindow(const QString datagram) {
    ui->message_win0->addItem(datagram);
}

void MainWindow::DisplayMessageOnMessageWindow(const QString datagram, const int count) { //тест
    ui->message_win0->addItem("Сообщение номер: " + datagram);
}

void MainWindow::OnSendClicked() {
    emit SendCalled(ui->send_message->text(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

void MainWindow::DisplayAddressPortOnYourIpYourPortLines(const QHostAddress server_address, const quint16 server_port) {
    ui->your_ip0->setText(server_address.toString());
    ui->your_port0->setText(QString::number(server_port));
}

void MainWindow::OnDatabaseWindowClicked() {
    emit DatabaseWindowCalled();
}


