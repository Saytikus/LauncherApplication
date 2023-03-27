#include "DataBaseWindow.h"
#include "ui_DataBaseWindow.h"

DataBaseWindow::DataBaseWindow(QWidget *parent) : QWidget(parent), ui(new Ui::DataBaseWindow) {
    ui->setupUi(this);

    QStringList headers;
    headers.append("логин");
    headers.append("пароль");
    this->SetupModel("profiles", headers);
    this->Create();
}

DataBaseWindow::~DataBaseWindow() {
    delete ui;
}

void DataBaseWindow::SetupModel(const QString table_name, const QStringList headers) {
    model = new QSqlTableModel(this);
    model->setTable(table_name);
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void DataBaseWindow::Create() {
    ui->database_table->setModel(model);
    model->select();
}

void DataBaseWindow::Refresh() {
    model->select();
}

void DataBaseWindow::Show() {
    this->show();
}

int DataBaseWindow::on_delete_2_clicked() {
    if (QMessageBox::warning(this,"Удаление записи", "Вы уверены, что хотите удалить эту запись?",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
    }
    else {
        emit DeleteRequest("profiles", ui->delete_profile->text());
        this->Refresh();
    }
    return 0;
}

void DataBaseWindow::HandleAnswerRequest(const int answer) {
    qDebug() << answer;
    if(answer == AnswerVariants::success) {
        QMessageBox::warning(this, "Уведомление", "Удаление прошло успешно!");
        this->Refresh();
    }
    if(answer == AnswerVariants::failure) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить профиль!");
    }
}
