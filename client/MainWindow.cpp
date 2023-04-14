#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::DisplayMessageOnMessageWindow(QString datagram) {
    ui->message_window->addItem(datagram);
}

void MainWindow::OnSendClicked() {
    emit SendCalled(ui->send_message->text(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

void MainWindow::DisplayClientPortOnMyPortLine(const quint16 port) {
    ui->my_port->setText(QString::number(port));
}

void MainWindow::Show() {
    this->show();
}

void MainWindow::closeEvent(QCloseEvent *event) { // знаю, что работает только на мейн окно клиента
    event->ignore();
    emit ConnectionMustBeFinished();
    event->accept();
}
