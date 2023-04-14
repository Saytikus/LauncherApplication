#include "RegistrationWindow.h"
#include "ui_Registration.h"

RegistrationWindow::RegistrationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::RegistrationWindow) {
    ui->setupUi(this);

}

RegistrationWindow::~RegistrationWindow() {
    delete ui;
}

void RegistrationWindow::OnAuthorizationWindowClicked() {
    emit AuthorizationWindowCalled();
    this->close();
}

int RegistrationWindow::OnRegistrationClicked() {
    QString entered_login = ui->reg_login->text();
    QString entered_pass = ui->reg_password->text();

    if(entered_login.isEmpty() || entered_pass.isEmpty()) {
        ui->registration->setEnabled(false);
        ui->reg_result_message->append("<font color=red>" + QString("Пустое поле логина или пароля") + "</font>");
        this->Delay(3000);
        ui->registration->setEnabled(true);
        ui->reg_result_message->clear();
        return -1;
    }
    if(entered_login == entered_pass) {
        ui->registration->setEnabled(false);
        ui->reg_result_message->append("<font color=red>" + QString("Логин и пароль не должны совпадать") + "</font>");
        this->Delay(3000);
        ui->registration->setEnabled(true);
        ui->reg_result_message->clear();
        return -1;
    }
    // Позже разделить методы проверки логина и пароля на несколько, чтобы выводить более информативное сообщение об ошибке
    if(!LoginCheck(entered_login) || !PasswordCheck(entered_pass)) {
        ui->registration->setEnabled(false);
        ui->reg_result_message->append("<font color=red>" + QString("Неподходящий логин или пароль") + "</font>");
        this->Delay(3000);
        ui->registration->setEnabled(true);
        ui->reg_result_message->clear();
        return -1;
    }

    emit RegistrationRequested(entered_login, entered_pass);
    return 0;
}

void RegistrationWindow::Show() {
    this->show();
}

void RegistrationWindow::HandleAnswer(const int reg_answer) {
    switch (reg_answer) {
    case ServerAnswerVariants::FAILURE:
        ui->registration->setEnabled(false);
        ui->reg_result_message->append("<font color=red>" + QString("Введённый логин уже занят!") + "</font>");
        this->Delay(3000);
        ui->registration->setEnabled(true);
        ui->reg_result_message->clear();
        break;

    case ServerAnswerVariants::SUCCESS:
        ui->registration->setEnabled(false);
        ui->reg_result_message->append("<font color=green>" + QString("Регистрация прошла успешно!") + "</font>");
        ui->reg_result_message->append("<font color=green>" + QString("Возвращаемся в окно авторизации...") + "</font>");
        this->Delay(3000);

        ui->reg_result_message->clear();
        ui->reg_login->clear();
        ui->reg_password->clear();
        ui->registration->setEnabled(true);

        this->close();
        emit AuthorizationWindowCalled();
        break;
    }
}

void RegistrationWindow::Delay(const int ms) {
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(ms);
    connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start();
    loop.exec();
}

// методы ниже будут перенесены в класс логики(наверное)
bool RegistrationWindow::LoginCheck(const QString login) {
    if(login.size() > 15 || login.size() < 5)
        return false;
    if(!login[0].isLetter() || login[0].isLower())
        return false;
    if(!ContainsDigit(login) || ContainsPunctuationSymbols(login) ||
       ContainsSpace(login))
        return false;
    return true;
}

bool RegistrationWindow::PasswordCheck(const QString password) {
    if(password.size() > 20 || password.size() < 8)
        return false;
    if(!password[0].isLetter() || password[0].isLower())
        return false;
    if(!ContainsDigit(password) || ContainsPunctuationSymbols(password) ||
       ContainsSpace(password))
        return false;
    return true;
}

bool RegistrationWindow::ContainsDigit(const QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isDigit())
            return true;
    }
    return false;
}
bool RegistrationWindow::ContainsLetter(const QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isLetter())
            return true;
    }
    return false;
}

bool RegistrationWindow::ContainsPunctuationSymbols(const QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isPunct())
            return true;
    }
    return false;
}
bool RegistrationWindow::ContainsSpace(const QString check_string) {
    for(QChar symbol : check_string) {
        if(symbol.isSpace())
            return true;
    }
    return false;
}
