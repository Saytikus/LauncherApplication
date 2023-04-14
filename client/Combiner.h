#ifndef COMBINER_H
#define COMBINER_H

#include <QObject>

#include "QUdpClient.h"
#include "MainWindow.h"
#include "AuthorizationWindow.h"
#include "RegistrationWindow.h"

class Combiner : public QObject {
    Q_OBJECT
public:
    explicit Combiner(QObject *parent = nullptr);
    void Combine();

    QUdpClient *client_;
    MainWindow *main_window_;
    AuthorizationWindow *authorization_window_;
    RegistrationWindow *registration_window_;
};

#endif // COMBINER_H
