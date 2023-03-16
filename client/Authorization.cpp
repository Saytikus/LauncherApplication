#include "Authorization.h"
#include "ui_Authorization.h"

Authorization::Authorization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
}

Authorization::~Authorization()
{
    delete ui;
}
