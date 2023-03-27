#include "Combiner.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Combiner combi;

    combi.auth_win_->show();

    combi.main_win_->show();

    combi.Combine();
    return a.exec();
}
