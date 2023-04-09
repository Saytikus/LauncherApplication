#include "Combiner.h"

Combiner::Combiner(QObject *parent) : QObject{parent} {
    server_ = new QUdpServer();
    interface_ = new Interface();

    connect(server_, SIGNAL(SocketBinded(const QHostAddress, const quint16)),
            interface_, SLOT(DisplayAddressPort(const QHostAddress, const quint16)));

    server_->Bind(QHostAddress::LocalHost);

    app_db_ = new AppDataBase();
    app_db_->test_call_field_name("profiles");
    app_db_->test();

    db_win_ = new DataBaseWindow();
    buf_pool_ = new ClientBufferPool();
    handler_thread_pool_ = new DataHandlerThreadPool();
    handler_thread_pool_->Start();
}

void Combiner::Combine() {
    connect(server_, SIGNAL(ReceivePocket(const QString)), interface_, SLOT(DisplayMessage(const QString)));
    connect(server_, SIGNAL(ReceivePocket(const QString, const int)),
            interface_, SLOT(DisplayMessage(const QString, const int))); //тест


    connect(interface_, SIGNAL(SendClicked(const QString, const QHostAddress, const quint16)),
            server_, SLOT(SendCall(const QString, const QHostAddress, const quint16)));

    connect(handler_thread_pool_, SIGNAL(RequestCreateProfile(const QString, const QString, const QString, const QString)),
            app_db_, SLOT(HandleRequestInsert(const QString, const QString, const QString, const QString)), Qt::DirectConnection);
    connect(app_db_, SIGNAL(AnswerRequestInsert(const int, const QString)),
            handler_thread_pool_, SLOT(SendAnswerCreateProfile(const int, const QString)), Qt::DirectConnection);

    connect(handler_thread_pool_, SIGNAL(RequestExistsProfile(const QString, const QString, const QString, const QString)),
            app_db_, SLOT(HandleRequestExists(const QString, const QString, const QString, const QString)), Qt::DirectConnection);
    connect(app_db_, SIGNAL(AnswerRequestExists(const int, const QString)),
            handler_thread_pool_, SLOT(SendAnswerExistsProfile(const int, const QString)), Qt::DirectConnection);

    connect(interface_, SIGNAL(DataBaseWinCall()), db_win_, SLOT(Show()), Qt::DirectConnection);
    connect(app_db_, SIGNAL(TableUpdate()), db_win_, SLOT(Refresh()), Qt::DirectConnection);

    connect(db_win_, SIGNAL(DeleteRequest(const QString, const QString)), app_db_, SLOT(HandleRequestDelete(const QString, const QString)), Qt::DirectConnection);
    connect(app_db_, SIGNAL(AnswerRequestDelete(const int)), db_win_, SLOT(HandleAnswerRequest(const int)), Qt::DirectConnection);

    connect(server_, SIGNAL(ConnectionStarted(const QHostAddress, const quint16)),
            buf_pool_, SLOT(CreateClientBuffer(const QHostAddress, const quint16)), Qt::DirectConnection);

    connect(server_, SIGNAL(ReceivePocket(const QByteArray, const int, const QString)),
            buf_pool_, SLOT(WriteReadBuffer(const QByteArray, const int, const QString)), Qt::DirectConnection);
    connect(buf_pool_, SIGNAL(ReadBufferChanged(const QBuffer*, const QString)),
            handler_thread_pool_, SLOT(ReadBufferChange(const QBuffer*, const QString)), Qt::DirectConnection);

    connect(handler_thread_pool_, SIGNAL(FormCompleteAnswerFinished(const QByteArray, const int, const QString)),
            buf_pool_, SLOT(WriteSendBuffer(const QByteArray, const int, const QString)), Qt::DirectConnection);
    connect(buf_pool_, SIGNAL(SendBufferChanged(const QBuffer*, const QString)),
            server_, SLOT(SendBufferChange(const QBuffer*, const QString)), Qt::DirectConnection);

    connect(handler_thread_pool_, SIGNAL(HandleReadBufferFinished(const QString)),
            buf_pool_, SLOT(ClearReadBuffer(const QString)), Qt::DirectConnection);
    connect(server_, SIGNAL(HandleSendBufferFinished(const QString)),
            buf_pool_, SLOT(ClearSendBuffer(const QString)), Qt::DirectConnection);

    connect(handler_thread_pool_, SIGNAL(RequestDeleteBuffer(const QString)),
            buf_pool_, SLOT(DeleteClientBuffer(const QString)), Qt::DirectConnection);
}
