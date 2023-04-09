#include "AppDataBase.h"

AppDataBase::AppDataBase(QObject *parent) : QObject{parent} {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("AppDataBase.db");

    query = new QSqlQuery(db);

    if(!db.open())
        qDebug() << db.lastError().text();

    if(!query->exec("CREATE TABLE IF NOT EXISTS profiles "
                "(login VARCHAR(15) NOT NULL,"
                "password VARCHAR(20) NOT NULL)"))
        qDebug() << query->lastError().text();
}

bool AppDataBase::InsertValues(const QString table_name, const QString fields, const QString values) {
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
        if(!query->exec("INSERT INTO " + table_name + " (" + fields + ") VALUES ('" + values_list[i] + "','" + values_list[i + 1] + "')")) {
            return false;
            qDebug() << query->lastError().text();
        }
    }
    return true;
}

bool AppDataBase::DeleteValues(const QString table_name, const QString first_field, const QString first_values) {
    // сделать нормальную обработку исключений для table_name, first_field и first_values
    QStringList first_values_list = first_values.split("!");
    for(int i = 0; i < first_values_list.size(); i++) {
        if(!query->exec("DELETE FROM " + table_name + " WHERE (" + first_field + " = '" + first_values_list[i] + "')")) {
            return false;
            qDebug() << query->lastError().text();
        }
    }
    return true;
}

void AppDataBase::test_call_field_name(const QString table_name) {
    QSqlRecord re = db.record(table_name);
    qDebug() << re.fieldName(0); // вывод названия первой строки
}

void AppDataBase::test() {
    qDebug() << "Начинается вывод значений из строк login & password таблицы profiles:";
    query->exec("SELECT * FROM profiles");
    while(query->next()) {
        qDebug() << query->value("login").toString() << query->value("password").toString();
        qDebug();
    }
}

void AppDataBase::HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id) {
    if(!profile_data.contains("!")) {
        qDebug() << "Ошибка в создании profile_data (AppThread)";
        exit(1);
    }
    qDebug() << "HandleRequestInsert запущен";
    if(table_name == "profiles") {
        query->exec("SELECT login FROM profiles");
        QStringList login_list;
        while(query->next()) {
            login_list << query->value(0).toString();
        }
        qDebug() << "HandleRequestInsert - login_list: " << login_list; //

        QStringList exist_check_list = profile_data.split("!");
        if(login_list.contains(exist_check_list[0]))
            emit AnswerRequestInsert(AnswerVariants::FAILURE, buffer_id);
        else {
            if(this->InsertValues(table_name, fields, profile_data)) {
                emit AnswerRequestInsert(AnswerVariants::SUCCESS, buffer_id);
                emit TableUpdate();
            }
        }
    }
}

void AppDataBase::HandleRequestExists(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id) {
    qDebug() << "Поток в обработчике БД: " << QThread::currentThread();
    if(!auth_data.contains("!")) {
        qDebug() << "Ошибка в создании auth_data (AppThread)";
        exit(1);
    }
    if(table_name == "profiles") {
        query->exec("SELECT * FROM profiles");
        QStringList login_list;
        QStringList password_list;
        while(query->next()) {
            login_list << query->value("login").toString();
            password_list << query->value("password").toString();
        }
        qDebug() << "HandleRequestExists - login_list: " << login_list; //

        QStringList exist_check_list = auth_data.split("!");
        if(!login_list.contains(exist_check_list[0])) {
            emit AnswerRequestExists(AnswerVariants::FAILURE, buffer_id);
        }
        if(login_list.contains(exist_check_list[0])){
            if(password_list.contains(exist_check_list[1]) && password_list.indexOf(exist_check_list[1]) == login_list.indexOf(exist_check_list[0]))
                emit AnswerRequestExists(AnswerVariants::SUCCESS, buffer_id);
            else
                emit AnswerRequestExists(AnswerVariants::PASS_ERR, buffer_id);
        }
    }
}

void AppDataBase::HandleRequestDelete(const QString table_name, const QString first_field_data) {
    if(table_name == "profiles") {
        query->exec("SELECT login FROM profiles");
        QStringList login_list;
        while(query->next())
            login_list << query->value("login").toString();
        qDebug() << "HandleRequestDelete - login_list: " << login_list; //

        if(!login_list.contains(first_field_data))
            emit AnswerRequestDelete(AnswerVariants::FAILURE);

        else if(this->DeleteValues(table_name, "login", first_field_data))
            emit AnswerRequestDelete(AnswerVariants::SUCCESS);
    }
}
