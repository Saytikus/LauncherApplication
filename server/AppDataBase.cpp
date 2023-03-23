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

void AppDataBase::InsertValues(const QString table_name, const QString fields, const QString values) {
    // сделать нормальную обработку исключений для table_name, fields и values
    if(!fields.contains(",") || fields.split(",").size() % 2 != 0)
        qDebug() << "Ошибка в строке полей таблицы";

    QStringList values_list = values.split("!");   
    if(!values.contains("!") || values_list.size() % 2 != 0)
        qDebug() << "Ошибка в строке значений добавляемых в БД";

    for(int i = 0; i < values_list.size() - 1; i++) {
        if(!query->exec("INSERT INTO " + table_name + " (" + fields + ") VALUES ('" + values_list[i] + "','" + values_list[i + 1] + "')"))
            qDebug() << query->lastError().text();
    }
}

void AppDataBase::DeleteValues(const QString table_name, const QString first_field, const QString first_values) {
    // сделать нормальную обработку исключений для table_name, first_field и first_values
    QStringList first_values_list = first_values.split("!");
    for(int i = 0; i < first_values_list.size(); i++) {
        if(!query->exec("DELETE FROM " + table_name + " WHERE (" + first_field + " = '" + first_values_list[i] + "')"))
            qDebug() << query->lastError().text();
    }
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

void AppDataBase::HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data) {
    if(!profile_data.contains("!")) {
        qDebug() << "Ошибка в создании profile_data (AppThread)";
        exit(1);
    }
    if(table_name == "profiles") {
        query->exec("SELECT login FROM profiles");
        QStringList login_list;
        while(query->next()) {
            login_list << query->value(0).toString();
        }
        qDebug() << "HandleRequestInsert - login_list: " << login_list; //

        QStringList exist_check_list = profile_data.split("!");
        if(login_list.contains(exist_check_list[0]))
            emit AnswerRequestInsert("Введённый логин уже занят!");
        else {
            this->InsertValues(table_name, fields, profile_data);
            emit AnswerRequestInsert("Регистрация прошла успешно!");
        }
    }
}

void AppDataBase::HandleRequestExists(const QString table_name, const QString fields, const QString auth_data) {
    if(!auth_data.contains("!")) {
        qDebug() << "Ошибка в создании auth_data (AppThread)";
        exit(1);
    }
    if(table_name == "profiles") {
        query->exec("SELECT login FROM profiles");
        QStringList login_list;
        while(query->next()) {
            login_list << query->value(0).toString();
        }
        qDebug() << "HandleRequestExists - login_list: " << login_list; //

        QStringList exist_check_list = auth_data.split("!");
        if(!login_list.contains(exist_check_list[0])) {
            emit AnswerRequestExists("Такого пользователя не существует");
        }
        else {
            emit AnswerRequestExists("Такой пользователь существует");
        }
    }
}
