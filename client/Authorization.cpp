#include "Authorization.h"
#include "ui_Authorization.h"

Authorization::Authorization(QWidget *parent) : QWidget(parent), ui(new Ui::Authorization) {
    ui->setupUi(this);

}

Authorization::~Authorization() {
    delete ui;
}

void Authorization::AuthWinShow() {
    this->show();
}

void Authorization::on_in_reg_win_clicked() {
    emit RegWinCall();
    this->close();
}

void Authorization::on_sign_in_clicked() {
    emit AuthStart(ui->login->text(), ui->password->text());
}

void Authorization::AcceptAuthAnswer(const QString auth_answer) {
    if(auth_answer == "Такого пользователя не существует") {
        ui->auth_result_message->append("<font color=red>" + QString("Такого пользователя не существует") + "</font>");
        ui->sign_in->setEnabled(false);
        this->Delay(3000);
        ui->sign_in->setEnabled(true);
        ui->auth_result_message->clear();
    }
    if(auth_answer == "Неверный пароль") {
        ui->auth_result_message->append("<font color=red>" + QString("Неверный пароль") + "</font>");
        ui->sign_in->setEnabled(false);
        this->Delay(3000);
        ui->sign_in->setEnabled(true);
        ui->auth_result_message->clear();
    }
    if(auth_answer == "Такой пользователь существует") {
        ui->auth_result_message->append("<font color=green>" + QString("Вы успешно вошли!") + "</font>");
        ui->auth_result_message->append("<font color=green>" + QString("Запускаем приложение...") + "</font>");
        ui->sign_in->setEnabled(false);
        this->Delay(3000);

        ui->auth_result_message->clear();
        this->close();
        emit AuthSuccessful();
    }
}

void Authorization::Delay(const int ms) {
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(ms);
    connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start();
    loop.exec();
}
