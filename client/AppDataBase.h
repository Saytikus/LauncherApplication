#ifndef APPDATABASE_H
#define APPDATABASE_H

#include <QObject>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class AppDataBase : public QObject
{
    Q_OBJECT
public:
    explicit AppDataBase(QObject *parent = nullptr);

signals:

};

#endif // APPDATABASE_H
