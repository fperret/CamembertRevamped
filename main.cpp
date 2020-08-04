#include "mymainwidget.h"

#include <QtCharts>
#include <QApplication>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication l_app(argc, argv);
    QMainWindow l_window;
    MyMainWidget l_mainWidget;

    l_window.setCentralWidget(&l_mainWidget);
    l_window.resize(900, 900);
    l_window.show();
    return l_app.exec();
}
