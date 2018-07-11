#include "mainwindow.h"
#include <QApplication>
#include<QFontDatabase>
#include"singleinstance.h"
int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);
    // Set application information
    a.setApplicationName ("NeatMusic");
    a.setApplicationVersion ("0.0.1");
    // Load fonts from resources
    QFontDatabase::addApplicationFont (":/fonts/roboto-hinted/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont (":/fonts/roboto-hinted/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont (":/fonts/roboto-hinted/Roboto-Regular.ttf");
    QString name=QStringLiteral("NeatMusic");
    SingleInstance instance;
    if(instance.hasPrevious(name))
        return EXIT_SUCCESS;
    instance.listen(name);
    MainWindow w;
    w.show();
    QObject::connect(&instance,&SingleInstance::newInstance,[&](){
       (&w)->setWindowToFront(true);
    });

    return a.exec();
}
