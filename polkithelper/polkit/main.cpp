#include <QCoreApplication>
#include "polkithelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PolkitHelper sample(argc, argv);

    return a.exec();
}
