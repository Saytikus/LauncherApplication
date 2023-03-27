#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QListWidget>
#include <QHostAddress>

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
    void on_send_clicked();

    void DisplayMessage(const QString datagram);
    void DisplayMessage(const QString datagram, const int count); //тест

    void DisplayAddressPort(const QHostAddress server_address, const quint16 server_port);

    void on_database_win_call_clicked();

private:
    Ui::Interface *ui;

signals:
    void SendClicked(const QString message, const QHostAddress address, const quint16 port);

    void DataBaseWinCall();
};

#endif // INTERFACE_H
