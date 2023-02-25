#ifndef MAINWINDOWCSSH_H
#define MAINWINDOWCSSH_H

#include <QMainWindow>
#include <QProcess>
#include <QList>
#include <QToolBar>

typedef QList<QProcess> QProcessList;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowCSSH; }
QT_END_NAMESPACE

class MainWindowCSSH : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowCSSH(QWidget *parent = nullptr);
    virtual ~MainWindowCSSH();

public slots:
    void executeCommand();
    void about();
    void showGlobalLicense();
    void showWarrantyLicense();
    void showCopyLicense();
private:
    void makeConnections();
    Ui::MainWindowCSSH *ui;
    QProcessList processList;
    QToolBar *mainToolBar;
};
#endif // MAINWINDOWCSSH_H
