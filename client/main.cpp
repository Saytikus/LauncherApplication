#include "Combiner.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Combiner combi;
    combi.authorization_window_->show();
    combi.main_window_->show(); //
    combi.Combine();

    return a.exec();
}
