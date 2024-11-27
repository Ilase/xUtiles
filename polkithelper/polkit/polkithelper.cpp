#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "polkithelper.h"
#include "PolkitAdaptor.h"

#define MINUTE 30000

PolkitHelper::PolkitHelper(int &argc, char **argv) : QCoreApplication(argc, argv)
{
    (void) new PolkitAdaptor(this);

    // Register the DBus service
    if (!QDBusConnection::systemBus().registerService("org.qt.policykit.rootwrite"))
    {
        QTextStream ErrStream(stderr);
        ErrStream << QDBusConnection::systemBus().lastError().message();

        QTimer::singleShot(0, this, SLOT(quit()));
        return;
    }

    if (!QDBusConnection::systemBus().registerObject("/", this))
    {
        QTextStream ErrStream(stderr);
        ErrStream << "unable to register service interface to dbus";

        QTimer::singleShot(0, this, SLOT(quit()));
        return;
    }
    // Normally you will set a timeout so your application can
    // free some resources of the poor client machine ;)
    QTimer::singleShot(MINUTE, this, SLOT(quit()));
}

PolkitHelper::~PolkitHelper()
{
}

int PolkitHelper::write(const QString& path, const QString& content, QString &strErrMsg)
{
    // message().service() is the service name of the caller
    // We can check if the caller is authorized to the following action
    PolkitQt1::Authority::Result result;
    PolkitQt1::SystemBusNameSubject subject(message().service());

    result = PolkitQt1::Authority::instance()->checkAuthorizationSync("org.qt.policykit.rootwrite.write", subject , PolkitQt1::Authority::AllowUserInteraction);
    if (result == PolkitQt1::Authority::Yes)
    {   // Caller is authorized so we can perform the action
        return writeValue(path, content, strErrMsg);
    }
    else
    {   // Caller is not authorized so the action can't be performed
        return 1;
    }
}

int PolkitHelper::writeValue(const QString& path, const QString& content, QString &strErrMsg)
{
    // This action must be authorized first. It will set the implicit
    // authorization for the Shout action by editing the .policy file
    try
    {
        QFileInfo FileInfo(path);

        QFile File(path);
        if(!File.open(QIODevice::WriteOnly))
        {
           strErrMsg = "File(" + FileInfo.fileName() + ") Open Error: " + File.errorString();
           return -1;
        }
        QTextStream OutStream(&File);
        OutStream << content;

        File.close();
    }
    catch (QException &err)
    {
        strErrMsg = err.what();
    }

    return 0;
}
