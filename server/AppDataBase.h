#ifndef APPDATABASE_H
#define APPDATABASE_H

#include <QObject>

class AppDataBase : public QObject
{
    Q_OBJECT
public:
    explicit AppDataBase(QObject *parent = nullptr);

signals:

};

#endif // APPDATABASE_H
