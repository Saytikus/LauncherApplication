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
    void on_bind0_clicked();
    void on_unbind0_clicked();
    void on_send_clicked();
    void on_read0_clicked();

    void DisplayMessage(const QString datagram);
    void DisplayMessage(const QString datagram, const int count); //тест

private:
    Ui::Interface *ui;

signals:
    void BindClicked(const QHostAddress, const quint16 port);
    void UnbindClicked();
    void ReadClicked();
    void SendClicked(const QString message, const QHostAddress address, const quint16 port);
};

#endif // INTERFACE_H
