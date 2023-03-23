#include "Registration.h"
#include "ui_Registration.h"

Registration::Registration(QWidget *parent) : QWidget(parent), ui(new Ui::Registration) {
    ui->setupUi(this);

}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_return_aut_win_clicked() {
    emit AuthWinCall();
    this->close();
}

int Registration::on_registration_clicked() {
    QString entered_login = ui->reg_login->text();
    QString entered_pass = ui->reg_password->text();

    if(entered_login.isEmpty() || entered_pass.isEmpty()) {
        ui->reg_result_message->append("<font color=red>" + QString("Пустое поле логина или пароля") + "</font>");
        return -1;
    }
    // Позже разделить методы проверки логина и пароля на несколько, чтобы выводить более информативное сообщение об ошибке
    if(!LoginCheck(entered_login) || !PassCheck(entered_pass)) {
        ui->reg_result_message->append("<font color=red>" + QString("Неподходящий логин или пароль") + "</font>");
        return -1;
    }

    emit RegRequest(entered_login, entered_pass);
    return 0;
}

void Registration::RegWinShow() {
    this->show();
}

void Registration::AcceptRegAnswer(const QString reg_answer) {
    if(reg_answer == "Введённый логин уже занят!") {
        ui->reg_result_message->clear();
        ui->reg_result_message->append("<font color=red>" + QString("Введённый логин уже занят!") + "</font>");
    }
    else {
        ui->reg_result_message->append("<font color=green>" + QString("Регистрация прошла успешно!") + "</font>");
        ui->reg_result_message->append("<font color=green>" + QString("Возвращаемся в окно авторизации...") + "</font>");
        // Спёр код для задержки
        QEventLoop loop;
        QTimer timer;
        timer.setInterval(3000);
        connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start();
        loop.exec();

        ui->reg_result_message->clear();
        this->close();
        emit AuthWinCall();
    }
}

// методы ниже будут перенесены в класс логики(наверное)
bool Registration::LoginCheck(const QString login) {
    if(login.size() > 15 || login.size() < 5)
        return false;
    if(!login[0].isLetter() || login[0].isLower())
        return false;
    if(!ContainsDigit(login) || ContainsPunctSym(login) ||
       ContainsSpace(login))
        return false;
    return true;
}

bool Registration::PassCheck(const QString password) {
    if(password.size() > 20 || password.size() < 8)
        return false;
    if(!password[0].isLetter() || password[0].isLower())
        return false;
    if(!ContainsDigit(password) || ContainsPunctSym(password) ||
       ContainsSpace(password))
        return false;
    return true;
}

bool Registration::ContainsDigit(QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isDigit())
            return true;
    }
    return false;
}
bool Registration::ContainsLetter(QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isLetter())
            return true;
    }
    return false;
}

bool Registration::ContainsPunctSym(QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isPunct())
            return true;
    }
    return false;
}
bool Registration::ContainsSpace(QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isSpace())
            return true;
    }
    return false;
}
