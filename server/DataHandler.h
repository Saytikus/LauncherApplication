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

    Q_INVOKABLE void HandleClientRequest(const QBuffer *read_buffer, const QString buffer_id);
    Q_INVOKABLE void HandleAnswerCreateProfile(const int answer, const QString buffer_id);
    Q_INVOKABLE void HandleAnswerExistsProfile(const int answer, const QString buffer_id);

private:

private slots:

signals:
    void RequestCreateProfileToPool(const QString table_name, const QString fields, const QString profile_data, const QString buffer_id);
    void RequestExistsProfileToPool(const QString table_name, const QString fields, const QString auth_data, const QString buffer_id);
    void SendCompleteAnswerToPool(const QByteArray complete_answer, const int size, const QString buffer_id);
    void HandleClientRequestFinished(const QString buffer_id);
    void RequestDeleteBufferToPool(const QString buffer_id);
};

#endif // DATAHANDLER_H
