#include "Combiner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Combiner combi;

    combi.main_win_->show();

    combi.Combine();

    return a.exec();
}
