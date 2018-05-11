#include "mainwindow.h"
#include <QApplication>
#include<QFontDatabase>
int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);
    // Set application information
    a.setApplicationName ("NeatMusic");
    a.setApplicationVersion ("1.0.0");
    // Load fonts from resources
    QFontDatabase::addApplicationFont (":/fonts/roboto-hinted/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont (":/fonts/roboto-hinted/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont (":/fonts/roboto-hinted/Roboto-Regular.ttf");
    MainWindow w;
    w.show();

    return a.exec();
}
