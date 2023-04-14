#ifndef COMBINER_H
#define COMBINER_H

#include <QObject>

#include "QUdpServer.h"
#include "MainWindow.h"
#include "ApplicationDatabase.h"
#include "DatabaseWindow.h"
#include "ClientBufferPool.h"
#include "DataHandlerThreadPool.h"

class Combiner : public QObject {
    Q_OBJECT
public:
    explicit Combiner(QObject *parent = nullptr);
    void Combine();

    QUdpServer *server_;
    MainWindow *main_window_;
    ApplicationDatabase *application_database_;
    DatabaseWindow *database_window_;
    ClientBufferPool *client_buffer_pool_;
    DataHandlerThreadPool *data_handler_thread_pool_;
signals:

};

#endif // COMBINER_H
