#ifndef QTUDPCLIENT_H
#define QTUDPCLIENT_H

#include <QWidget>
#include <QObject>
#include <QUdpSocket>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class QtUDPClient; }
QT_END_NAMESPACE

class QtUDPClient : public QWidget
{
    Q_OBJECT

public:
    QtUDPClient(QWidget *parent = nullptr);
    ~QtUDPClient();
    void Send(const QString message, const QHostAddress address, const quint16 port);

private slots:
    void on_Connect_clicked();
    void on_read_clicked();
    void on_unbind_clicked();

private:
    Ui::QtUDPClient *ui;
    QUdpSocket *socket_;

public slots:
    void Read();
    void ReadOne();
};
#endif // QTUDPCLIENT_H
