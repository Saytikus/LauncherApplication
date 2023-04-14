#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <QWidget>
#include <QEventLoop>
#include <QTimer>

namespace Ui {
class AuthorizationWindow;
}

class AuthorizationWindow : public QWidget {
    Q_OBJECT
public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);
    ~AuthorizationWindow();

private:
    Ui::AuthorizationWindow *ui;
    enum ServerAnswerVariants {
        FAILURE = -1,
        SUCCESS = 1,
        PASS_ERR = -2
    };

    void Delay(const int ms);

private slots:
    void Show();
    void HandleAnswer(const int auth_answer);

    void OnRegistrationWindowClicked();
    void OnSignInClicked();

signals:
    void AuthorizationStarted(const QString login, const QString password);
    void AuthorizationCompletedSuccessful();

    void RegistrationWindowCalled();
};

#endif // AUTHORIZATIONWINDOW_H
