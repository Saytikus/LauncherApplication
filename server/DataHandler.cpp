#include "DataHandler.h"

DataHandler::DataHandler(QObject *parent) : QObject{parent} {

}

void DataHandler::HandleClientRequest(const QBuffer *read_buffer) {
    qDebug() << "Поток в HandleClientRequest: " << QThread::currentThread(); //

    qDebug() << "Содержание read_buffer: " << read_buffer->data(); //
    QStringList request = QString(read_buffer->data()).split("|");

    if(request.size() == 4 &&
       request[0] == "reg_start" && request[3] == "reg_end") {
        QString profile_data = request[1] + "!" + request[2];
        //emit RequestCreateProfile("profiles", "login, password", profile_data);
    }

    if(request.size() == 4 &&
       request[0] == "auth_start" && request[3] == "auth_end") {
        QString auth_data = request[1] + "!" + request[2];
        //emit RequestExistsProfile("profiles", "login, password", auth_data);
    }
}

void DataHandler::HandleAnswerCreateProfile(const int answer) {
    qDebug() << "Поток в HandleAnswerCreateProfile: " << QThread::currentThread(); //
    qDebug() << "Сформированный ответ: " << "reg_answer_start|" + QString::number(answer) + "|reg_answer_end"; //
    //emit SendCompleteAnswer("reg_answer_start|" + QString::number(answer) + "|reg_answer_end");
}

void DataHandler::HandleAnswerExistsProfile(const int answer) {
    qDebug() << "Поток в HandleAnswerExistsProfile: " << QThread::currentThread(); //
    //emit SendCompleteAnswer("auth_answer_start|" + QString::number(answer) + "|auth_answer_end");
}
