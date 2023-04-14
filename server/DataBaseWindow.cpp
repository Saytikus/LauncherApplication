#include "DatabaseWindow.h"
#include "ui_DataBaseWindow.h"

DatabaseWindow::DatabaseWindow(QWidget *parent) : QWidget(parent), ui(new Ui::DatabaseWindow) {
    ui->setupUi(this);

    QStringList headers;
    headers.append("логин");
    headers.append("пароль");
    this->SetupModel("profiles", headers);
    this->CreateModel();
}

DatabaseWindow::~DatabaseWindow() {
    delete ui;
}

void DatabaseWindow::SetupModel(const QString table_name, const QStringList headers) {
    model = new QSqlTableModel(this);
    model->setTable(table_name);
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void DatabaseWindow::CreateModel() {
    ui->database_table->setModel(model);
    model->select();
}

void DatabaseWindow::RefreshModel() {
    model->select();
}

void DatabaseWindow::Show() {
    this->show();
}

int DatabaseWindow::OnDeleteButtonClicked() {
    if (QMessageBox::warning(this,"Удаление записи", "Вы уверены, что хотите удалить эту запись?",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
    }
    else {
        emit DeleteRequested("profiles", ui->delete_profile->text());
        this->RefreshModel();
    }
    return 0;
}

void DatabaseWindow::HandleAnswer(const int answer) {
    qDebug() << answer;
    if(answer == DatabaseAnswerVariants::SUCCESS) {
        QMessageBox::warning(this, "Уведомление", "Удаление прошло успешно!");
        this->RefreshModel();
    }
    if(answer == DatabaseAnswerVariants::FAILURE) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить профиль!");
    }
}
