#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QWidget>

#include <QSqlTableModel>

#include <QMessageBox>

#include <QDebug>

namespace Ui {
class DataBaseWindow;
}

class DataBaseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DataBaseWindow(QWidget *parent = nullptr);
    ~DataBaseWindow();

private slots:
    void Refresh();
    void Show();

    int on_delete_2_clicked();
    void HandleAnswerRequest(const int answer);
private:
    Ui::DataBaseWindow *ui;
    QSqlTableModel *model;

    void SetupModel(const QString table_name, const QStringList headers);
    void Create();

    enum AnswerVariants {
        failure,
        success
    };

signals:
    void DeleteRequest(const QString table_name, const QString first_field_data);
};

#endif // DATABASEWINDOW_H
