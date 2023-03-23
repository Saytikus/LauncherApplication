#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    server_ = new QUdpServer();
    interface_ = new Interface();

    connect(server_, SIGNAL(SocketBinded(const QHostAddress, const quint16)),
            interface_, SLOT(DisplayAddressPort(const QHostAddress, const quint16)));

    server_->Bind(QHostAddress::LocalHost);

    app_db_ = new AppDataBase();
    app_db_->test_call_field_name("profiles");
    //app_db_->InsertValues("profiles", "login, password", "testUser!testPass");
    //app_db_->DeleteValues("profiles", "login", "testUser!testUser!testUser!testUser");
    app_db_->test();
}

void Combiner::Combine() {
    connect(server_, SIGNAL(ReceivePocket(const QString)), interface_, SLOT(DisplayMessage(const QString)));
    connect(server_, SIGNAL(ReceivePocket(const QString, const int)),
            interface_, SLOT(DisplayMessage(const QString, const int))); //тест


    connect(interface_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            server_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));

    connect(server_, SIGNAL(RequestCreateProfile(const QString, const QString, const QString)),
            app_db_, SLOT(HandleRequestInsert(const QString, const QString, const QString)), Qt::DirectConnection);
    connect(app_db_, SIGNAL(AnswerRequestInsert(const QString)),
            server_, SLOT(AcceptAnswerInsert(const QString)), Qt::DirectConnection);

    connect(server_, SIGNAL(RequestExistsProfile(const QString, const QString, const QString)),
            app_db_, SLOT(HandleRequestExists(const QString, const QString, const QString)), Qt::DirectConnection);
    connect(app_db_, SIGNAL(AnswerRequestExists(const QString)),
            server_, SLOT(AcceptAnswerExists(const QString)), Qt::DirectConnection);
}
