#include "AppDataBase.h"

AppDataBase::AppDataBase(QObject *parent) : QObject{parent} {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("AppDataBase.db");

    if(!db.open())
        qDebug() << db.lastError().text();

    if(!query.exec("CREATE TABLE IF NOT EXISTS users "
                   "(login VARCHAR(15), "
                   "password VARCHAR(20)"))
        qDebug() << query.lastError().text();
}

void AppDataBase::InsertValues(const QString table_name, const QString values) {
    QStringList values_list = values.split("!");
    if(query.exec("INSERT INTO" + table_name + " (Ip, Port) VALUES (" + values_list[0] + "," + values_list[1] + ")"))
        qDebug() << query.lastError().text();
}

void AppDataBase::DeleteValues(const QString table_name, const QString values) {

}
