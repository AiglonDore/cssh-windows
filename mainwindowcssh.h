#ifndef MAINWINDOWCSSH_H
#define MAINWINDOWCSSH_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowCSSH; }
QT_END_NAMESPACE

class MainWindowCSSH : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowCSSH(QWidget *parent = nullptr);
    ~MainWindowCSSH();

private:
    Ui::MainWindowCSSH *ui;
};
#endif // MAINWINDOWCSSH_H
