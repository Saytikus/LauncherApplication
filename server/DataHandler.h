#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <QBuffer>
#include <QStringList>
#include <QDebug>

#include <QThread> //

class DataHandler : public QObject {
    Q_OBJECT
public:
    explicit DataHandler(QObject *parent = nullptr);

    Q_INVOKABLE void HandleClientRequest(const QBuffer *read_buffer);

private:

private slots:
    void HandleAnswerCreateProfile(const int answer);
    void HandleAnswerExistsProfile(const int answer);

signals:
    /*void RequestCreateProfile(const QString table_name, const QString fields, const QString profile_data);
    void RequestExistsProfile(const QString table_name, const QString fields, const QString auth_data);
    void SendCompleteAnswer(const QString complete_answer);*/
};

#endif // DATAHANDLER_H
