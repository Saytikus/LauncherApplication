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

    Q_INVOKABLE void HandleClientRequest(const QBuffer *read_buffer, const quint16 buffer_id);

private:
    quint16 buffer_id_ = 0;
private slots:
    void HandleAnswerCreateProfile(const int answer);
    void HandleAnswerExistsProfile(const int answer);

signals:
    void RequestCreateProfileToPool(const QString table_name, const QString fields, const QString profile_data);
    void RequestExistsProfileToPool(const QString table_name, const QString fields, const QString auth_data);
    void SendCompleteAnswerToPool(const QByteArray complete_answer, const int size, const quint16 buffer_id);
};

#endif // DATAHANDLER_H
