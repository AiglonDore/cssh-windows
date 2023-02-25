#include "mainwindowcssh.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool translated(false);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "cssh-windows_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            translated = true;
            break;
        }
    }
    if (!translated)
    {
        const QString baseName = "cssh-windows_en_EN";
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
        }
    }
    MainWindowCSSH w;
    w.showMaximized();
    return a.exec();
}
