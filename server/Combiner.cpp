#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    server_ = new QUdpServer();

    interface_ = new Interface();
}

void Combiner::Combine() {
    connect(server_, SIGNAL(ReceivePocket(const QString)), interface_, SLOT(DisplayMessage(const QString)));
    connect(server_, SIGNAL(ReceivePocket(const QString, const int)),
            interface_, SLOT(DisplayMessage(const QString, const int))); //тест


    connect(interface_, SIGNAL(BindClicked(const QHostAddress, const quint16)),
            server_, SLOT(BindCall(const QHostAddress, const quint16)));
    connect(interface_, SIGNAL(UnbindClicked()), server_, SLOT(UnbindCall()));
    connect(interface_, SIGNAL(ReadClicked()), server_, SLOT(ReadClicked()));
    connect(interface_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            server_, SLOT(SendClicked(const QString, const QHostAddress, const quint16)));

}
