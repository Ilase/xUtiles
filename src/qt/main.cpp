#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--discard") == 0){
            system("bash /opt/xUtils/bin/discard_changes.sh");
            exit(0);
        }

    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
