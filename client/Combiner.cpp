#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    client_ = new QUdpClient();
    client_->Bind(QHostAddress::LocalHost, 7777);

    main_win_ = new MainWindow();

    auth_win_ = new Authorization();

}

void Combiner::Combine() {
    connect(client_, SIGNAL(ReceivePocket(const QString)), main_win_, SLOT(DisplayMessage(const QString)));
    connect(main_win_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            client_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));
}
