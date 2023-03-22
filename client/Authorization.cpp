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
    if(auth_answer == "Такого пользователя не существует")
        ui->auth_result_message->append("<font color=red>" + QString("Такого пользователя не существует") + "</font>");

    else {
        ui->auth_result_message->append("<font color=green>" + QString("Вы успешно вошли!") + "</font>");
        ui->auth_result_message->append("<font color=green>" + QString("Запускаем приложение...") + "</font>");
        // Спёр код для задержки
        QEventLoop loop;
        QTimer timer;
        timer.setInterval(3000);
        connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start();
        loop.exec();

        this->close();
        emit AuthSuccessful();
    }
}
