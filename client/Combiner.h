#ifndef COMBINER_H
#define COMBINER_H

#include <QObject>

#include "QUdpClient.h"
#include "MainWindow.h"
#include "Authorization.h"
#include "Registration.h"

class Combiner : public QObject {
    Q_OBJECT
public:
    explicit Combiner(QObject *parent = nullptr);
    void Combine();

    QUdpClient *client_;
    MainWindow *main_win_;
    Authorization *auth_win_;
    Registration *reg_win_;

private:

signals:

};

#endif // COMBINER_H
