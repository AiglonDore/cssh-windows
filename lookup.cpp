#include "lookup.h"

#include <QList>
#include <QDir>

#ifdef _WIN32
bool sshDetected(const QString& appName)
{
    QStringList path = qEnvironmentVariable("path").split(';',Qt::SkipEmptyParts);
    path << QDir::toNativeSeparators("C:/WINDOWS/System32/OpenSSH");
    for (const QString& dir : path)
    {
        QDir directory(dir);
        if (directory.entryList(QDir::Files|QDir::Executable).contains(appName))
            return true;
    }
    return false;
}
#else
bool sshDectected(const QString& appName)
{
    QStringList path = qEnvironmentVariable("PATH").split(':',Qt::SkipEmptyParts);
    for (const QString& dir : path)
    {
        QDir directory(dir);
        if (directory.entryList(QDir::Files|QDir::Executable|QDir::CaseSensitive).contains(appName))
            return true;
    }
    return false;
}
#endif
