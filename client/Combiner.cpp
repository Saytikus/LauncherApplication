#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    client_ = new QUdpClient();
    quint16 my_port = QRandomGenerator::global()->bounded(1025, 9999);

    main_win_ = new MainWindow();

    connect(client_, SIGNAL(SocketWasBinded(const quint16)),
            main_win_, SLOT(DisplayMyPort(const quint16))); //

    client_->Bind(QHostAddress::LocalHost, my_port);

    //main_win_ = new MainWindow();

    auth_win_ = new Authorization();

}

void Combiner::Combine() {
    connect(client_, SIGNAL(ReceivePocket(const QString)), main_win_, SLOT(DisplayMessage(const QString)));
    connect(main_win_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            client_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));

    //connect(client_, SIGNAL(SocketWasBinded(const quint16)),
    //        main_win_, SLOT(DisplayMyPort(const quint16))); //
}
