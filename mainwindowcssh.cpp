#include "mainwindowcssh.h"
#include "ui_mainwindowcssh.h"
#include <QDir>
#include <QFile>
#include <QString>
#include <QMessageBox>

#ifdef _WIN32
bool sshDetected()
{
    const QString appName = "ssh.exe";
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
bool sshDectected()
{
    const QString appName = "ssh";
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

MainWindowCSSH::MainWindowCSSH(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowCSSH)
{
    ui->setupUi(this);
}

MainWindowCSSH::~MainWindowCSSH()
{
    delete ui;
}

