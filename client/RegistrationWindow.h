#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QWidget {
    Q_OBJECT
public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();

private slots:
    void OnAuthorizationWindowClicked();
    int OnRegistrationClicked();

    void Show();
    void HandleAnswer(const int reg_answer);
private:
    Ui::RegistrationWindow *ui;
    enum ServerAnswerVariants {
        FAILURE = -1,
        SUCCESS = 1,
        PASS_ERR = -2
    };

    void Delay(const int ms);

    bool LoginCheck(const QString login);
    bool PasswordCheck(const QString password);
    bool ContainsDigit(const QString check_string);
    bool ContainsLetter(const QString check_string);
    bool ContainsPunctuationSymbols(const QString check_string);
    bool ContainsSpace(const QString check_string);
signals:
    void AuthorizationWindowCalled();
    void RegistrationRequested(const QString login, const QString password);
};

#endif // REGISTRATIONWINDOW_H
