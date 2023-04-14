#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHostAddress>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);

private slots:
    void DisplayMessageOnMessageWindow(const QString datagram);
    void DisplayClientPortOnMyPortLine(const quint16 port);
    void Show();

    void OnSendClicked();
signals:
    void ConnectionMustBeFinished();
    void SendCalled(const QString message, const QHostAddress address, const quint16 port);
};

#endif // MAINWINDOW_H
