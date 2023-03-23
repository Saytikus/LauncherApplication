#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    client_ = new QUdpClient();
    quint16 my_port = QRandomGenerator::global()->bounded(1025, 9999);

    main_win_ = new MainWindow();

    connect(client_, SIGNAL(SocketBinded(const quint16)),
            main_win_, SLOT(DisplayMyPort(const quint16))); //

    client_->Bind(QHostAddress::LocalHost, my_port);
    client_->HandShakeStart();

    //main_win_ = new MainWindow();

    auth_win_ = new Authorization();

    reg_win_ = new Registration();

    connect(auth_win_, SIGNAL(RegWinCall()), reg_win_, SLOT(RegWinShow()));
    connect(reg_win_, SIGNAL(AuthWinCall()), auth_win_, SLOT(AuthWinShow()));

}

void Combiner::Combine() {
    connect(client_, SIGNAL(ReceivePocket(const QString)), main_win_, SLOT(DisplayMessage(const QString)));
    connect(main_win_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            client_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));

    connect(reg_win_, SIGNAL(RegRequest(const QString, const QString)),
            client_, SLOT(RegRequestSend(const QString, const QString)));
    connect(client_, SIGNAL(ReceiveRegAnswer(const QString)),
            reg_win_, SLOT(AcceptRegAnswer(const QString)));

    connect(auth_win_, SIGNAL(AuthStart(const QString, const QString)),
            client_, SLOT(AuthRequestSend(const QString, const QString)));
    connect(client_, SIGNAL(ReceiveAuthAnswer(const QString)),
            auth_win_, SLOT(AcceptAuthAnswer(const QString)));
    connect(auth_win_, SIGNAL(AuthSuccessful()), main_win_, SLOT(MainWindowShow()));
}
