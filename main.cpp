#include "mainwindowcssh.h"
#include "lookup.h"

#include <QApplication>
#include <QCoreApplication>
#include <QScopedPointer>
#include <QLocale>
#include <QTranslator>
#include <QProcess>
#include <QString>
#include <QList>
#include <QDir>
#include <QStringBuilder>

#include <QTextStream>

const QStringList noGuiCommands = {"ssh", "scp", "bscp", "rscp"};
QTextStream cerr(stderr);
QTextStream cin(stdin);
QTextStream cout(stdout);

namespace NoGUI {
    void ssh(int argc, char *argv[])
    {
        if (argc < 3)
        {
            cerr << argv[0] << QObject::tr(": missing arguments. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        if (!sshDetected())
        {
            cerr << argv[0] << QObject::tr(": SSH not detected. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        QProcess sshProcess;
        sshProcess.setProgram(noGuiCommands[0]);
        QStringList args;
        for (int i = 2; i < argc; i++)
            args << argv[i];
        sshProcess.setArguments(args);
        sshProcess.setInputChannelMode(QProcess::ForwardedInputChannel);
        sshProcess.setProcessChannelMode(QProcess::ForwardedChannels);
        sshProcess.start();
        sshProcess.waitForFinished(-1);
    }

    void scp(int argc, char *argv[])
    {
        if (argc < 4)
        {
            cerr << argv[0] << QObject::tr(": missing arguments. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        if (!sshDetected("scp"))
        {
            cerr << argv[0] << QObject::tr(": SCP not detected. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        QProcess sshProcess;
        sshProcess.setProgram(noGuiCommands[1]);
        QStringList args;
        for (int i = 2; i < argc; i++)
        {
            args << argv[i];
        }
        sshProcess.setArguments(args);
        sshProcess.setInputChannelMode(QProcess::ForwardedInputChannel);
        sshProcess.setProcessChannelMode(QProcess::ForwardedChannels);
        sshProcess.start();
        sshProcess.waitForFinished(-1);
    }

    void bscp(int argc, char *argv[])
    {
        if (argc < 4)
        {
            cerr << argv[0] << QObject::tr(": missing arguments. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        if (!sshDetected("scp"))
        {
            cerr << argv[0] << QObject::tr(": SCP not detected. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        QProcess sshProcess;
        sshProcess.setProgram(noGuiCommands[1]);
        QStringList args;
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] == '-') args << argv[i];
        }
        sshProcess.setInputChannelMode(QProcess::ForwardedInputChannel);
        sshProcess.setProcessChannelMode(QProcess::ForwardedChannels);
        bool fst(false);
        for (int i = 2; i < argc; i++)
        {
            if (!fst && argv[i][0] != '-') args << argv[i];
            else if (argv[i][0] != '-')
            {
                args << argv[i];
                sshProcess.setArguments(args);
                sshProcess.start();
                sshProcess.waitForFinished(-1);
                args.pop_back();
            }
        }
    }

    void rscp(int argc, char *argv[])
    {
        if (argc < 4)
        {
            cerr << argv[0] << QObject::tr(": missing arguments. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        if (!sshDetected("scp"))
        {
            cerr << argv[0] << QObject::tr(": SCP not detected. Aborting.").toLocal8Bit() << "\n";
            exit(1);
        }
        QProcess sshProcess;
        sshProcess.setProgram(noGuiCommands[1]);
        QStringList args;
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] == '-') args << argv[i];
        }
        sshProcess.setInputChannelMode(QProcess::ForwardedInputChannel);
        sshProcess.setProcessChannelMode(QProcess::ForwardedChannels);
        QStringList remoteDirs;
        QString localDir;
        for (int i = 2; i < argc; i++)
        {
            if (QString(argv[i]).contains('@')) remoteDirs << argv[i];
            else if (localDir.isEmpty() && argv[i][0] != '-') localDir = argv[i];
        }
        for (const QString& dir : remoteDirs)
        {
            args << dir;
            QString hostname = dir.split('@').last();
            QStringList localDirSplit = QDir::fromNativeSeparators(localDir).split('/');
            localDirSplit.last() = hostname % "_" % localDirSplit.last();
            localDir = QDir::toNativeSeparators(localDirSplit.join('/'));
            sshProcess.setArguments(args);
            sshProcess.start();
            sshProcess.waitForFinished(-1);
            args.pop_back();
            args.pop_back();
        }
    }
};
QCoreApplication *createApplication(int &argc, char *argv[])
{
    if (argc >= 2 && noGuiCommands.contains(argv[1]))
        return new QCoreApplication(argc, argv);
    return new QApplication(argc, argv);
}


int main(int argc, char *argv[])
{
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    bool translated(false);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "cssh-windows_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app->installTranslator(&translator);
            translated = true;
            break;
        }
    }
    if (!translated)
    {
        const QString baseName = "cssh-windows_fr_FR";
        if (translator.load(":/i18n/" + baseName)) {
            app->installTranslator(&translator);
        }
    }
    if (qobject_cast<QApplication *>(app.data()))
    {
        MainWindowCSSH w;
        w.showMaximized();
        return app->exec();
    }
    else
    {
        if (argc == 2)
        {
            cerr << argv[0] << QObject::tr(": no arguments found. Aborting.").toLocal8Bit() << "\n";
            return 1;
        }
        switch (noGuiCommands.indexOf(argv[1])) {
        case 0:
            NoGUI::ssh(argc,argv);
            break;
        case 1:
            NoGUI::scp(argc,argv);
            break;
        case 2:
            NoGUI::bscp(argc,argv);
            break;
        default:
            NoGUI::rscp(argc,argv);
            break;
        }
        return 0;
    }
}
