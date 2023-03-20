#ifndef APPDATABASE_H
#define APPDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class AppDataBase : public QObject
{
    Q_OBJECT
public:
    explicit AppDataBase(QObject *parent = nullptr);

private:
    QSqlDatabase db;
    QSqlQuery query;

private slots:
    void InsertValues(const QString table_name, const QString values);
    void DeleteValues(const QString table_name, const QString values);
signals:

};

#endif // APPDATABASE_H
