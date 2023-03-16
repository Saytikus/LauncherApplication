#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    client_ = new QUdpClient();
    client_->Bind(QHostAddress::LocalHost, 7777);

    main_win = new MainWindow();

    auth_win = new Authorization();

}

void Combiner::Combine() {
    connect(client_, SIGNAL(ReceivePocket(const QString)), main_win, SLOT(DisplayMessage(const QString)));
    connect(main_win, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            client_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));
}
