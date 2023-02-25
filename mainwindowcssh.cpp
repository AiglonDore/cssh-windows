#include "mainwindowcssh.h"
#include "ui_mainwindowcssh.h"

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

