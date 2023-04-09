#include "DataHandler.h"

DataHandler::DataHandler(QObject *parent) : QObject{parent} {

}

void DataHandler::HandleClientRequest(const QBuffer *read_buffer, const QString buffer_id) {
    qDebug() << "Поток в HandleClientRequest: " << QThread::currentThread(); //

    qDebug() << "id буфера на входе: " << buffer_id;
    qDebug() << "Содержание read_buffer: " << read_buffer->data(); //
    QStringList request = QString(read_buffer->data()).split("|");

    if(request.size() == 4 &&
       request[0] == "reg_start" && request[3] == "reg_end") {
        QString profile_data = request[1] + "!" + request[2];
        emit RequestCreateProfileToPool("profiles", "login, password", profile_data, buffer_id);
    }

    if(request.size() == 4 &&
       request[0] == "auth_start" && request[3] == "auth_end") {
        QString auth_data = request[1] + "!" + request[2];
        emit RequestExistsProfileToPool("profiles", "login, password", auth_data, buffer_id);
    }
    if(request.size() == 2 &&
       request[0] == "request") {
        emit RequestDeleteBufferToPool(buffer_id);
    }
    emit HandleClientRequestFinished(buffer_id);
}

void DataHandler::HandleAnswerCreateProfile(const int answer, const QString buffer_id) {
    qDebug() << "Поток в HandleAnswerCreateProfile: " << QThread::currentThread(); //
    qDebug() << "Сформированный ответ: " << "reg_answer_start|" + QString::number(answer) + "|reg_answer_end"; //
    QByteArray complete_answer = ("reg_answer_start|" + QString::number(answer) + "|reg_answer_end").toUtf8();
    emit SendCompleteAnswerToPool(complete_answer, complete_answer.size(), buffer_id);
}

void DataHandler::HandleAnswerExistsProfile(const int answer, const QString buffer_id) {
    qDebug() << "Поток в HandleAnswerExistsProfile: " << QThread::currentThread(); //
    QByteArray complete_answer = ("auth_answer_start|" + QString::number(answer) + "|auth_answer_end").toUtf8();
    emit SendCompleteAnswerToPool(complete_answer, complete_answer.size(), buffer_id);
}
