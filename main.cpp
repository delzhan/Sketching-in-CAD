#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));

    MainWindow window;
    window.show();
    return app.exec();
}
