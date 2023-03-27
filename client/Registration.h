#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_return_aut_win_clicked();
    int on_registration_clicked();

    void RegWinShow();
    void AcceptRegAnswer(const QString reg_answer);

private:
    Ui::Registration *ui;

    void Delay(const int ms);

    enum AnswerVariants {
        failure,
        success
    };

    bool LoginCheck(const QString login);
    bool PassCheck(const QString password);

    bool ContainsDigit(QString check_string);
    bool ContainsLetter(QString check_string);
    bool ContainsPunctSym(QString check_string);
    bool ContainsSpace(QString check_string);
signals:
    void AuthWinCall();
    void RegRequest(const QString login, const QString password);
};

#endif // REGISTRATION_H
