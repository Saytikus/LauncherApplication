#include "Combiner.h"

#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Combiner combi;
    combi.Combine();

    combi.interface_->show();

    // Тестовый код
    /*
    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
        qDebug() << "Error driver!";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test1.db");

    if(!db.open())
        qDebug() << db.lastError().text();

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS test1 "
           "(Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
            "Ip VARCHAR(40) NOT NULL, "
               " Port VARCHAR(4) NOT NULL)");
    for(int i = 1; i < 12; i++) {
        if(!query.exec(QString("DELETE FROM test1 where (Id = " + QString::number(i) + ")")))
            qDebug() << query.lastError();
    }

    if(!query.exec("INSERT INTO test1 (Ip, Port) VALUES ('121.0.0.1', '7777')"))
        qDebug() << query.lastError().text();
    query.exec("INSERT INTO test1(Ip, Port) VALUES('121.0.0.1','2415')");

    query.exec("SELECT * FROM test1");
    while(query.next()) {
        qDebug() << query.value("Id").toString() << query.value("Ip").toString() << query.value("Port").toString();
        qDebug();
    }
    qDebug() << db.tables();
    db.close();*/

    return a.exec();
}
