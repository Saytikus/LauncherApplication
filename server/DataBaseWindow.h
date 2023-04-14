#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QWidget>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class DatabaseWindow;
}

class DatabaseWindow : public QWidget {
    Q_OBJECT
public:
    explicit DatabaseWindow(QWidget *parent = nullptr);
    ~DatabaseWindow();

private slots:
    void RefreshModel();
    void Show();

    int OnDeleteButtonClicked();
    void HandleAnswer(const int answer);
private:
    Ui::DatabaseWindow *ui;
    QSqlTableModel *model;

    void SetupModel(const QString table_name, const QStringList headers);
    void CreateModel();

    enum DatabaseAnswerVariants {
        FAILURE = -1,
        SUCCESS = 1,
    };

signals:
    void DeleteRequested(const QString table_name, const QString first_field_data);
};

#endif // DATABASEWINDOW_H
