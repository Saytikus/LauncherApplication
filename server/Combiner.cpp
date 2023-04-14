#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    server_ = new QUdpServer();
    main_window_ = new MainWindow();

    connect(server_, SIGNAL(SocketBinded(const QHostAddress, const quint16)),
            main_window_, SLOT(DisplayAddressPortOnYourIpYourPortLines(const QHostAddress, const quint16)));

    server_->Bind(QHostAddress::LocalHost);

    application_database_ = new ApplicationDatabase();

    database_window_ = new DatabaseWindow();
    client_buffer_pool_ = new ClientBufferPool();
    data_handler_thread_pool_ = new DataHandlerThreadPool();
    data_handler_thread_pool_->StartHandlerThreads();
}

void Combiner::Combine() {
    connect(server_, SIGNAL(ReceivePocket(const QString)),
            main_window_, SLOT(DisplayMessageOnMessageWindow(const QString)));
    connect(server_, SIGNAL(ReceivePocket(const QString, const int)),
            main_window_, SLOT(DisplayMessageOnMessageWindow(const QString, const int))); //тест


    connect(main_window_, SIGNAL(SendCalled(const QString, const QHostAddress, const quint16)),
            server_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));

    connect(data_handler_thread_pool_, SIGNAL(RequestCreateProfile(const QString, const QString, const QString, const QString)),
            application_database_, SLOT(HandleRequestInsert(const QString, const QString, const QString, const QString)), Qt::DirectConnection);
    connect(application_database_, SIGNAL(AnswerRequestInsertFinished(const int, const QString)),
            data_handler_thread_pool_, SLOT(SendAnswerCreateProfile(const int, const QString)), Qt::DirectConnection);

    connect(data_handler_thread_pool_, SIGNAL(RequestExistsProfile(const QString, const QString, const QString, const QString)),
            application_database_, SLOT(HandleRequestExists(const QString, const QString, const QString, const QString)), Qt::DirectConnection);
    connect(application_database_, SIGNAL(AnswerRequestExistsFinished(const int, const QString)),
            data_handler_thread_pool_, SLOT(SendAnswerExistsProfile(const int, const QString)), Qt::DirectConnection);

    connect(main_window_, SIGNAL(DatabaseWindowCalled()), database_window_, SLOT(Show()), Qt::DirectConnection);
    connect(application_database_, SIGNAL(TableUpdated()), database_window_, SLOT(RefreshModel()), Qt::DirectConnection);

    connect(database_window_, SIGNAL(DeleteRequested(const QString, const QString)),
            application_database_, SLOT(HandleRequestDelete(const QString, const QString)), Qt::DirectConnection);
    connect(application_database_, SIGNAL(AnswerServerRequestDeleteFinished(const int)),
            database_window_, SLOT(HandleAnswer(const int)), Qt::DirectConnection);

    connect(server_, SIGNAL(ConnectionStarted(const QHostAddress, const quint16)),
            client_buffer_pool_, SLOT(CreateClientBuffer(const QHostAddress, const quint16)), Qt::DirectConnection);

    connect(server_, SIGNAL(ReceivePocket(const QByteArray, const int, const QString)),
            client_buffer_pool_, SLOT(WriteReadBuffer(const QByteArray, const int, const QString)), Qt::DirectConnection);
    connect(client_buffer_pool_, SIGNAL(ReadBufferChanged(const QBuffer*, const QString)),
            data_handler_thread_pool_, SLOT(SendReadBufferChangeToHandler(const QBuffer*, const QString)), Qt::DirectConnection);

    connect(data_handler_thread_pool_, SIGNAL(CompleteAnswerFinished(const QByteArray, const int, const QString)),
            client_buffer_pool_, SLOT(WriteSendBuffer(const QByteArray, const int, const QString)), Qt::DirectConnection);
    connect(client_buffer_pool_, SIGNAL(SendBufferChanged(const QBuffer*, const QString)),
            server_, SLOT(SendBufferChange(const QBuffer*, const QString)), Qt::DirectConnection);

    connect(data_handler_thread_pool_, SIGNAL(HandleReadBufferFinished(const QString)),
            client_buffer_pool_, SLOT(ClearReadBuffer(const QString)), Qt::DirectConnection);
    connect(server_, SIGNAL(HandleSendBufferFinished(const QString)),
            client_buffer_pool_, SLOT(ClearSendBuffer(const QString)), Qt::DirectConnection);

    connect(data_handler_thread_pool_, SIGNAL(RequestDeleteBuffer(const QString)),
            client_buffer_pool_, SLOT(DeleteClientBuffer(const QString)), Qt::DirectConnection);
}
