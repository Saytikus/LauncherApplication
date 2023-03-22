#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //void TestDisplay(QString data);

private:
    Ui::MainWindow *ui;

private slots:
    void DisplayMessage(const QString datagram);

    void on_send_clicked();

    void DisplayMyPort(const quint16 port);

    void MainWindowShow();

signals:
    void SendClicked(const QString message, const QHostAddress address, const quint16 port);
};

#endif // MAINWINDOW_H
