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
    Ui::Interface *ui;
    QString datagram_;

private slots:
    void on_bind0_clicked();
    void on_unbind0_clicked();
    void on_read_one0_clicked();
    void on_read0_clicked();

    QString Disp(QString datagram);

private:
    QUdpServer *server;
    void Display(QString datagram);
};

#endif // INTERFACE_H
