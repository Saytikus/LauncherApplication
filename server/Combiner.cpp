#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    server_ = new QUdpServer();
    interface_ = new Interface();

    connect(server_, SIGNAL(SocketBinded(const QHostAddress, const quint16)),
            interface_, SLOT(DisplayAddressPort(const QHostAddress, const quint16)));

    server_->Bind(QHostAddress::LocalHost);
}

void Combiner::Combine() {
    connect(server_, SIGNAL(ReceivePocket(const QString)), interface_, SLOT(DisplayMessage(const QString)));
    connect(server_, SIGNAL(ReceivePocket(const QString, const int)),
            interface_, SLOT(DisplayMessage(const QString, const int))); //тест


    connect(interface_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            server_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));
}
