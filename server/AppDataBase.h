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

    enum AnswerVariants {
        failure,
        success
    };    

private slots:
    bool InsertValues(const QString table_name, const QString fields, const QString values);
    bool DeleteValues(const QString table_name, const QString first_field, const QString values);

    void HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data);
    void HandleRequestExists(const QString table_name, const QString fields, const QString auth_data);
    void HandleRequestDelete(const QString table_name, const QString first_field_data);

signals:
    void AnswerRequestInsert(const int answer);
    void AnswerRequestExists(const QString answer);
    void AnswerRequestDelete(const int answer);
    void TableUpdate();

};

#endif // APPDATABASE_H
