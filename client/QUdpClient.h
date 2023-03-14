#ifndef QUDPCLIENT_H
#define QUDPCLIENT_H

#include <QWidget>

#include <QUdpSocket>
#include <QHostAddress>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class QUdpClient; }
QT_END_NAMESPACE

class QUdpClient : public QWidget
{
    Q_OBJECT

public:
    QUdpClient(QWidget *parent = nullptr);
    ~QUdpClient();
    bool Bind(const QHostAddress address, quint16 port);
    void Send(const QString message, const quint16 port);
    void Read();

private slots:
    void on_send_clicked();

private:
    Ui::QUdpClient *ui;
    QUdpSocket *socket_;
};
#endif // QUDPCLIENT_H
