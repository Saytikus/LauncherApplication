#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>

#include "QUdpServer.h"
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Interface; }
QT_END_NAMESPACE

class Interface : public QWidget
{
    Q_OBJECT

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void on_bind0_clicked();
    void on_unbind0_clicked();

    //void on_read_one0_clicked(); заморожено до лучших времён
    void on_read0_clicked();

    void DisplayMessage(QString datagram);

private:
    Ui::Interface *ui;
    QUdpServer *server;
};

#endif // INTERFACE_H
