#ifndef MAINWINDOWCSSH_H
#define MAINWINDOWCSSH_H

#include <QMainWindow>
#include <QProcess>
#include <QList>
#include <QToolBar>
#include <QThread>

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
    void showNetwork();

private slots:
    void editMainConfigFile();
    void editConfigFile();
    void loadAndExecuteScript();
private:
    void makeConnections();
    Ui::MainWindowCSSH *ui;
    QProcessList processList;
    QToolBar *mainToolBar;
    QThread *networkThread;//TO activate the thread to resolve DNS
};
#endif // MAINWINDOWCSSH_H
