#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void OnSendClicked();
    void OnDatabaseWindowClicked();

    void DisplayMessageOnMessageWindow(const QString datagram);
    void DisplayMessageOnMessageWindow(const QString datagram, const int count); //тест
    void DisplayAddressPortOnYourIpYourPortLines(const QHostAddress server_address, const quint16 server_port);
private:
    Ui::MainWindow *ui;

signals:
    void SendCalled(const QString message, const QHostAddress address, const quint16 port);
    void DatabaseWindowCalled();
};

#endif // MAINWINDOW_H
