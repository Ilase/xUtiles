#ifndef POLKITHELPER_H
#define POLKITHELPER_H

#include <QDBusConnection>
#include <QDBusContext>
#include <QDBusMessage>
#include <QCoreApplication>
#include <polkit-qt5-1/polkitqt1-authority.h>
#include <polkit-qt5-1/polkitqt1-subject.h>

class PolkitHelper : public QCoreApplication, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.qt.policykit.rootwrite")

private:
    int writeValue(const QString& path, const QString& content, QString &strErrMsg);

public:
    PolkitHelper(int &argc, char **argv);
    ~PolkitHelper() override;

public Q_SLOTS:
    int write(const QString& path, const QString& content, QString &strErrMsg);
};


#endif // POLKITHELPER_H
