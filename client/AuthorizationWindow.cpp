#include "AuthorizationWindow.h"
#include "ui_AuthorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::AuthorizationWindow) {
    ui->setupUi(this);

}

AuthorizationWindow::~AuthorizationWindow() {
    delete ui;
}

void AuthorizationWindow::Show() {
    this->show();
}

void AuthorizationWindow::OnRegistrationWindowClicked() {
    emit RegistrationWindowCalled();
    this->close();
}

void AuthorizationWindow::OnSignInClicked() {
    emit AuthorizationStarted(ui->login->text(), ui->password->text());
}

void AuthorizationWindow::HandleAnswer(const int auth_answer) {
    switch (auth_answer) {

    case ServerAnswerVariants::FAILURE:
        ui->auth_result_message->append("<font color=red>" + QString("Такого пользователя не существует") + "</font>");
        ui->sign_in->setEnabled(false);
        this->Delay(3000);
        ui->sign_in->setEnabled(true);
        ui->auth_result_message->clear();
        break;

    case ServerAnswerVariants::PASS_ERR:
        ui->auth_result_message->append("<font color=red>" + QString("Неверный пароль") + "</font>");
        ui->sign_in->setEnabled(false);
        this->Delay(3000);
        ui->sign_in->setEnabled(true);
        ui->auth_result_message->clear();
        break;

    case ServerAnswerVariants::SUCCESS:
        ui->auth_result_message->append("<font color=green>" + QString("Вы успешно вошли!") + "</font>");
        ui->auth_result_message->append("<font color=green>" + QString("Запускаем приложение...") + "</font>");
        ui->sign_in->setEnabled(false);
        this->Delay(3000);

        ui->auth_result_message->clear();
        this->close();
        emit AuthorizationCompletedSuccessful();
        break;
    }
}

void AuthorizationWindow::Delay(const int ms) {
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(ms);
    connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start();
    loop.exec();
}
