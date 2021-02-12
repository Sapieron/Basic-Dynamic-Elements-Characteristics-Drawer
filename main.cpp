#include "widget/themewidget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("Drawer_PL");
    a.installTranslator(&translator);

    QMainWindow window;
    ThemeWidget *widget = new ThemeWidget();
    window.setCentralWidget(widget);
    window.resize(1000, 1000);
    window.show();
    return a.exec();
}

