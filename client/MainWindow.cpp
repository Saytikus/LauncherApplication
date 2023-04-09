#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::DisplayMessage(QString datagram) {
    ui->message_win->addItem(datagram);
}

void MainWindow::on_send_clicked() {
    emit SendClicked(ui->send_message->text(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

/*void MainWindow::TestDisplay(QString data) {
    ui->message_win->addItem("TestDisplay: " + data);
}*/

void MainWindow::DisplayMyPort(const quint16 port) {
    ui->my_port->setText(QString::number(port));
}

void MainWindow::MainWindowShow() {
    this->show();
}

void MainWindow::closeEvent(QCloseEvent *event) { // знаю, что работает только на мейн окно клиента
    event->ignore();
    emit ConnectionMustBeFinished();
    event->accept();
}
