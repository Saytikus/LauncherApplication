#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::DisplayMessage(const QString datagram) {
    ui->message_win->addItem(datagram);
}

void MainWindow::on_send_clicked() {
    emit SendClicked(ui->send_message->text(), QHostAddress::LocalHost, ui->send_port->value());
    ui->send_message->clear();
}

