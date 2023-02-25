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

void MainWindowCSSH::makeConnections()
{
    connect(ui->executePushButton,SIGNAL(clicked()),this,SLOT(executeCommand()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
}

MainWindowCSSH::MainWindowCSSH(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowCSSH)
{
    ui->setupUi(this);
    mainToolBar = new QToolBar(tr("Tool bar"));
    mainToolBar->addActions({ui->actionNew,ui->actionClose,ui->actionClose_all});
    mainToolBar->addSeparator();
    mainToolBar->addAction(ui->actionLoad_script);
    addToolBar(mainToolBar);
    mainToolBar->setMovable(false);
    makeConnections();
}

MainWindowCSSH::~MainWindowCSSH()
{
    delete mainToolBar;
    delete ui;
}

void MainWindowCSSH::executeCommand()
{
    QStringList commandWords = ui->commandLineEdit->text().split(' ',Qt::SkipEmptyParts);
    if (commandWords.isEmpty())
    {
        ui->statusbar->showMessage(tr("No command to execute."));
        return;
    }
    if (commandWords[0] == "ssh")
    {
        int countHosts(0);
        ui->statusbar->showMessage(tr("Adding %n new host.","",countHosts));
    }
    else
    {
        ui->statusbar->showMessage(tr("Executing command."));
    }
}

void MainWindowCSSH::about()
{
    QMessageBox::information(this,tr("Cluster SSH information"),tr("This is a tool to connect to a cluster of hosts, using SSH, on Linux and Windows hosts."),QMessageBox::Close,QMessageBox::Close);
}
