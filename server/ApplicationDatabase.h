#ifndef APPLICATIONDATABASE_H
#define APPLICATIONDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QThread> //

class ApplicationDatabase : public QObject {
    Q_OBJECT
public:
    explicit ApplicationDatabase(QObject *parent = nullptr);

private:
    QSqlDatabase database_;
    QSqlQuery *query_;
    enum DatabaseAnswerVariants {
        FAILURE = -1,
        SUCCESS = 1,
        PASS_ERR = -2
    };

    bool InsertValues(const QString table_name, const QString fields, const QString values);
    bool DeleteValues(const QString table_name, const QString first_field, const QString first_field_value);

private slots:
    void HandleRequestInsert(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id);
    void HandleRequestExists(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id);
    void HandleRequestDelete(const QString table_name, const QString first_field_data);

signals:
    void AnswerRequestInsertFinished(const int answer, const QString buffer_id);
    void AnswerRequestExistsFinished(const int answer, const QString buffer_id);
    void AnswerServerRequestDeleteFinished(const int answer);
    void TableUpdated();
};

#endif // APPLICATIONDATABASE_H
