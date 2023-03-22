#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include <QEventLoop>
#include <QTimer>

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

private:
    Ui::Authorization *ui;

private slots:
    void AuthWinShow();

    void on_in_reg_win_clicked();
    void on_sign_in_clicked();

    void AcceptAuthAnswer(const QString auth_answer);

signals:
    void RegWinCall();

    void AuthStart(const QString login, const QString password);
    void AuthSuccessful();
};

#endif // AUTHORIZATION_H
