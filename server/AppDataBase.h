#ifndef APPDATABASE_H
#define APPDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QThread> //

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
        FAILURE = -1,
        SUCCESS = 1,
        PASS_ERR = -2
    };

private slots:
    bool InsertValues(const QString table_name, const QString fields, const QString values);
    bool DeleteValues(const QString table_name, const QString first_field, const QString values);

    void HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id);
    void HandleRequestExists(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id);
    void HandleRequestDelete(const QString table_name, const QString first_field_data);

signals:
    void AnswerRequestInsert(const int answer, const QString buffer_id);
    void AnswerRequestExists(const int answer, const QString buffer_id);
    void AnswerRequestDelete(const int answer);
    void TableUpdate();

};

#endif // APPDATABASE_H
