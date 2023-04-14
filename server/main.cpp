#include "Combiner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Combiner combi;
    combi.Combine();

    combi.main_window_->show();
    return a.exec();
}
