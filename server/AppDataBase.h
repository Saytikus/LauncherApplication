#ifndef APPDATABASE_H
#define APPDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

class AppDataBase : public QObject
{
    Q_OBJECT
public:
    explicit AppDataBase(QObject *parent = nullptr);
    void test_call_field_name(const QString table_name);
    void test();

private:
    QSqlDatabase db;
    QSqlQuery *query;

private slots:
    void InsertValues(const QString table_name, const QString fields, const QString values);
    void DeleteValues(const QString table_name, const QString first_field, const QString values);

    void HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data);
    void HandleRequestExists(const QString table_name, const QString fields, const QString auth_data);

signals:
    void AnswerRequestInsert(const QString answer);
    void AnswerRequestExists(const QString answer);
};

#endif // APPDATABASE_H
