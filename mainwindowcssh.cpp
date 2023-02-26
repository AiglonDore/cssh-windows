#include "mainwindowcssh.h"
#include "ui_mainwindowcssh.h"
#include <QDir>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringBuilder>

#include "licencedialog.h"
#include "configfilewidget.h"
#include "lookup.h"
#include "exception.h"

void MainWindowCSSH::makeConnections()
{
    connect(ui->executePushButton,SIGNAL(clicked()),this,SLOT(executeCommand()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->actionComplete,SIGNAL(triggered()),this,SLOT(showGlobalLicense()));
    connect(ui->actionWarranty,SIGNAL(triggered()),this,SLOT(showWarrantyLicense()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(showCopyLicense()));
    connect(ui->actionShow_network_information,SIGNAL(triggered()),this,SLOT(showNetwork()));
    connect(ui->actionMain_config_file,SIGNAL(triggered()),this,SLOT(editMainConfigFile()));
    connect(ui->actionOther,SIGNAL(triggered()),this,SLOT(editConfigFile()));
    connect(ui->actionLoad_script,SIGNAL(triggered()),this,SLOT(loadAndExecuteScript()));
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
#ifdef _WIN32
    if (!sshDetected())
    {
        QMessageBox::critical(this,tr("SSH not detected","SSH is the name of a program"),tr("SSH executable cannot be found on PATH. This program is useless without it. You can install using the optional fetaures of Windows: it's the \"SSH-Client\" feature."),QMessageBox::Close,QMessageBox::Close);
    }
#else
    if (!sshDetected())
    {
        QProcess unixVersion;
        unixVersion.setProgram("lsb_release");
        unixVersion.setArguments({"-a"});
        unixVersion.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
        unixVersion.start();
        unixVersion.waitForFinished(5000);
        QString unixVersionOutput = unixVersion.readAll();
        QMessageBox::critical(this,tr("SSH not detected","SSH is the name of a program"),tr("SSH executable cannot be found on PATH. This program is useless without it. Here is your Linux version:")
                              % unixVersionOutput % tr("Check on Intenet which package you must install and how. Usually, it is named \"openssh-client\"."), QMessageBox::Close,QMessageBox::Close);
    }
#endif
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
        // TODO : add host
        try {
            int nbConenctions = connectionList.size();
            int nbNewConnctions = parseSSHConnections();
        } catch (const Exception& e) {
            QMessageBox::critical(this,tr("Error in creating connections"),e.getMessage(),QMessageBox::Close,QMessageBox::Close);
        }
    }
    else
    {
        ui->statusbar->showMessage(tr("Executing command."));
        // TODO: add execute command
    }
}

void MainWindowCSSH::about()
{
    QMessageBox::information(this,tr("Cluster SSH information"),tr("This is a tool to connect to a cluster of hosts, using SSH, on Linux and Windows hosts."),QMessageBox::Close,QMessageBox::Close);
}

void MainWindowCSSH::showGlobalLicense()
{
    LicenceDialog window(LicenceDialogType::Global,this->width()/2,this->height()/2,this);
    window.exec();
}
void MainWindowCSSH::showWarrantyLicense()
{
    LicenceDialog window(LicenceDialogType::Warranty,this->width()/2,this->height()/2,this);
    window.exec();
}
void MainWindowCSSH::showCopyLicense()
{
    LicenceDialog window(LicenceDialogType::Copy,this->width()/2,this->height()/2,this);
    window.exec();
}

void MainWindowCSSH::showNetwork()
{
    LicenceDialog window(LicenceDialogType::Network,this->width()/2,this->height()/2,this);
    window.exec();
}

void MainWindowCSSH::editMainConfigFile()
{
    QDir dir = QDir::home();
    dir.cd(".ssh");
    QString path = dir.absoluteFilePath("config");
    ConfigFileWidget window(path,this->width()/2,this->height()/2,this);
    window.exec();
}

void MainWindowCSSH::editConfigFile()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open SSH configuration file..."),QDir::homePath() + "/.ssh",tr("All files (*.*)"));
    if (path.isEmpty()) return;
    ConfigFileWidget window(path,this->width()/2,this->height()/2,this);
    window.exec();
}

void MainWindowCSSH::loadAndExecuteScript()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Load script file..."),QDir::homePath(),tr("Shell scripts (*.sh,*.bash);;PowerShell scripts (*.ps1);;Windows Command Batch Script (*.bat);;All files (*.*)"));
    if (path.isEmpty())
    {
        ui->statusbar->showMessage(tr("No script to be loaded."));
        return;
    }
    QFile script(path);
    QFileInfo scriptInfo(script);
    if (!script.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::critical(this,tr("Unable to open script"),tr("File at location \"") % path % tr("\" cannot be opened. Aborting task."),QMessageBox::Close,QMessageBox::Close);
        return;
    }
    if (!scriptInfo.isExecutable())
    {
        QMessageBox::StandardButton ans = QMessageBox::warning(this,tr("Executing a non-executable file"),tr("You are trying to execute a script that is not defined as executable. Please double check the file or change its permission. If you are sur of what you are doing, you can proceed."),QMessageBox::Apply|QMessageBox::Abort,QMessageBox::Abort);
        if (ans != QMessageBox::Apply)
        {
            ui->statusbar->showMessage(tr("Aborting task."));
            return;
        }
    }
    QByteArray scriptData = script.readAll();
    script.close();
    // TODO: Feed data to processes
    for (int i = 0; i < processList.size(); ++i)
    {

    }
}

int MainWindowCSSH::parseSSHConnections()
{
    int nb(0);
    QStringList commandWords = ui->commandLineEdit->text().split(' ',Qt::SkipEmptyParts);
    commandWords.pop_front();

    for (const QString& part : commandWords)
    {
        QStringList partSplit = part.split('@');
        if (partSplit.size() != 2) throw Exception(tr("Each SSH host must be written like \"user@domain:port\". Please check your input."));
        //if (partSplit[0].isEmpty()) partSplit[0] = TODO: add current username.
        QString currentUserName = partSplit[0];
        //TODO: add domains and IP addresses
    }

    return nb;
}
