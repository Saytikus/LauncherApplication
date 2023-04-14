#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    client_ = new QUdpClient();
    quint16 my_port = QRandomGenerator::global()->bounded(1025, 9999);

    main_window_ = new MainWindow();

    connect(client_, SIGNAL(SocketBinded(const quint16)),
            main_window_, SLOT(DisplayClientPortOnMyPortLine(const quint16))); //

    client_->Bind(QHostAddress::LocalHost, my_port);
    client_->HandShakeStart();

    authorization_window_ = new AuthorizationWindow();
    registration_window_ = new RegistrationWindow();

    connect(authorization_window_, SIGNAL(RegistrationWindowCalled()), registration_window_, SLOT(Show()));
    connect(registration_window_, SIGNAL(AuthorizationWindowCalled()), authorization_window_, SLOT(Show()));
}

void Combiner::Combine() {
    connect(client_, SIGNAL(PocketReceived(const QString)),
            main_window_, SLOT(DisplayMessageOnMessageWindow(const QString)));
    connect(main_window_, SIGNAL(SendCalled(const QString, const QHostAddress, const quint16)),
            client_, SLOT(Send(const QString, const QHostAddress, const quint16)));

    connect(registration_window_, SIGNAL(RegistrationRequested(const QString, const QString)),
            client_, SLOT(RegistrationRequestSend(const QString, const QString)));
    connect(client_, SIGNAL(RegistrationAnswerReceived(const int)),
            registration_window_, SLOT(HandleAnswer(const int)));

    connect(authorization_window_, SIGNAL(AuthorizationStarted(const QString, const QString)),
            client_, SLOT(AuthorizationRequestSend(const QString, const QString)));
    connect(client_, SIGNAL(AuthorizationAnswerReceived(const int)),
            authorization_window_, SLOT(HandleAnswer(const int)));
    connect(authorization_window_, SIGNAL(AuthorizationCompletedSuccessful()),
            main_window_, SLOT(Show()));

    connect(main_window_, SIGNAL(ConnectionMustBeFinished()),
            client_, SLOT(ConnectionClosedSend()));
}
