#include "ApplicationDatabase.h"

ApplicationDatabase::ApplicationDatabase(QObject *parent) : QObject{parent} {
    database_ = QSqlDatabase::addDatabase("QSQLITE");
    database_.setDatabaseName("AppDataBase.db");

    query_ = new QSqlQuery(database_);

    if(!database_.open())
        qDebug() << database_.lastError().text();

    if(!query_->exec("CREATE TABLE IF NOT EXISTS profiles "
                "(login VARCHAR(15) NOT NULL,"
                "password VARCHAR(20) NOT NULL)"))
        qDebug() << query_->lastError().text();
}

bool ApplicationDatabase::InsertValues(const QString table_name, const QString fields, const QString values) {
    // сделать нормальную обработку исключений для table_name, fields и values
    if(!fields.contains(",") || fields.split(",").size() % 2 != 0) {
        qDebug() << "Ошибка в строке полей таблицы";
        return false;
    }
    QStringList values_list = values.split("!");   
    if(!values.contains("!") || values_list.size() % 2 != 0) {
        return false;
        qDebug() << "Ошибка в строке значений добавляемых в БД";
    }
    for(int i = 0; i < values_list.size() - 1; i++) {
        if(!query_->exec("INSERT INTO " + table_name + " (" + fields + ") VALUES ('" + values_list[i] + "','" + values_list[i + 1] + "')")) {
            return false;
            qDebug() << query_->lastError().text();
        }
    }
    return true;
}

bool ApplicationDatabase::DeleteValues(const QString table_name, const QString first_field, const QString first_field_value) {
    // сделать нормальную обработку исключений для table_name, first_field и first_values
    if(!query_->exec("DELETE FROM " + table_name + " WHERE (" + first_field + " = '" + first_field_value + "')")) {
        return false;
        qDebug() << query_->lastError().text();
    }
    return true;
}

void ApplicationDatabase::HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id) {
    if(!profile_data.contains("!")) {
        qDebug() << "Ошибка в создании profile_data (AppThread)";
        exit(1);
    }
    qDebug() << "HandleRequestInsert запущен";
    if(table_name == "profiles") {
        query_->exec("SELECT login FROM profiles");
        QStringList login_list;
        while(query_->next()) {
            login_list << query_->value(0).toString();
        }
        qDebug() << "HandleRequestInsert - login_list: " << login_list; //

        QStringList exist_check_list = profile_data.split("!");
        if(login_list.contains(exist_check_list[0]))
            emit AnswerRequestInsertFinished(DatabaseAnswerVariants::FAILURE, buffer_id);
        else {
            if(this->InsertValues(table_name, fields, profile_data)) {
                emit AnswerRequestInsertFinished(DatabaseAnswerVariants::SUCCESS, buffer_id);
                emit TableUpdated();
            }
        }
    }
}

void ApplicationDatabase::HandleRequestExists(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id) {
    qDebug() << "Поток в обработчике БД: " << QThread::currentThread();
    if(!auth_data.contains("!")) {
        qDebug() << "Ошибка в создании auth_data (AppThread)";
        exit(1);
    }
    if(table_name == "profiles") {
        query_->exec("SELECT * FROM profiles");
        QStringList login_list;
        QStringList password_list;
        while(query_->next()) {
            login_list << query_->value("login").toString();
            password_list << query_->value("password").toString();
        }
        qDebug() << "HandleRequestExists - login_list: " << login_list; //

        QStringList exist_check_list = auth_data.split("!");
        if(!login_list.contains(exist_check_list[0])) {
            emit AnswerRequestExistsFinished(DatabaseAnswerVariants::FAILURE, buffer_id);
        }
        if(login_list.contains(exist_check_list[0])){
            if(password_list.contains(exist_check_list[1]) && password_list.indexOf(exist_check_list[1]) == login_list.indexOf(exist_check_list[0]))
                emit AnswerRequestExistsFinished(DatabaseAnswerVariants::SUCCESS, buffer_id);
            else
                emit AnswerRequestExistsFinished(DatabaseAnswerVariants::PASS_ERR, buffer_id);
        }
    }
}

void ApplicationDatabase::HandleRequestDelete(const QString table_name, const QString first_field_data) {
    if(table_name == "profiles") {
        query_->exec("SELECT login FROM profiles");
        QStringList login_list;
        while(query_->next())
            login_list << query_->value("login").toString();
        qDebug() << "HandleRequestDelete - login_list: " << login_list; //

        if(!login_list.contains(first_field_data))
            emit AnswerServerRequestDeleteFinished(DatabaseAnswerVariants::FAILURE);

        else if(this->DeleteValues(table_name, "login", first_field_data))
            emit AnswerServerRequestDeleteFinished(DatabaseAnswerVariants::SUCCESS);
    }
}
