#ifndef COMBINER_H
#define COMBINER_H

#include <QObject>

#include "QUdpServer.h"
#include "Interface.h"
#include "AppDataBase.h"

class Combiner : public QObject {
    Q_OBJECT
public:
    explicit Combiner(QObject *parent = nullptr);
    void Combine();

    QUdpServer *server_;
    Interface *interface_;

signals:

};

#endif // COMBINER_H
