#include "configfilewidget.h"
#include "ui_configfilewidget.h"

#include <QFile>
#include <QMessageBox>
#include <QStringBuilder>
#include <QSaveFile>
#include <QFileDialog>

ConfigFileWidget::ConfigFileWidget(const QString& filename, int width, int height, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigFileWidget),
    edited(false),
    saveButton(new QPushButton(tr("Save"))),
    saveAsButton(new QPushButton(tr("Save as..."))),
    discardButton(new QPushButton(tr("Discard"))),
    closeButton(new QPushButton(tr("Close"))),
    fileLocation(filename)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    ui->buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);
    saveButton->setDisabled(true);
    ui->buttonBox->addButton(saveAsButton,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(discardButton,QDialogButtonBox::DestructiveRole);
    ui->buttonBox->addButton(closeButton,QDialogButtonBox::ActionRole);
    connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
    connect(saveAsButton,SIGNAL(clicked()),this,SLOT(saveAs()));
    connect(discardButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeWindow()));
    resize(width,height);

    QFile file(fileLocation);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Unable to open file"),tr("File at location \"") % fileLocation % tr("\" cannot be opened. Aborting task."));
    }
    ui->plainTextEdit->setPlainText(file.readAll());
    file.close();
    connect(ui->plainTextEdit,SIGNAL(textChanged()),this,SLOT(textEdited()));
}

ConfigFileWidget::~ConfigFileWidget()
{
    delete saveButton;
    delete saveAsButton;
    delete discardButton;
    delete closeButton;
    delete ui;
}

void ConfigFileWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        QWidget::changeEvent(e);
        break;
    }
}

void ConfigFileWidget::textEdited()
{
    edited = true;
    saveButton->setEnabled(true);
    setWindowTitle("*"+windowTitle());
}

void ConfigFileWidget::save()
{
    QSaveFile saveFile(fileLocation);
    if (!saveFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Unable to open file"),tr("File at location \"") % fileLocation % tr("\" cannot be opened. Aborting task."));
        return;
    }
    saveFile.write(ui->plainTextEdit->toPlainText().toLocal8Bit());
    saveFile.commit();
    edited = false;
    saveButton->setDisabled(true);
    setWindowTitle(windowTitle().remove(0,1));
}

void ConfigFileWidget::saveAs()
{
    QString newLocation = QFileDialog::getSaveFileName(this,tr("Save file as"),fileLocation,tr("All files (*.*)"));
    if (newLocation.isEmpty()) return;
    QSaveFile saveFile(newLocation);
    if (!saveFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Unable to open file"),tr("File at location \"") % fileLocation % tr("\" cannot be opened. Aborting task."));
        return;
    }
    saveFile.write(ui->plainTextEdit->toPlainText().toLocal8Bit());
    saveFile.commit();
    edited = false;
    saveButton->setDisabled(true);
    setWindowTitle(windowTitle().remove(0,1));
    fileLocation = newLocation;
}

void ConfigFileWidget::closeEvent(QCloseEvent *e)
{
    if (edited)
    {
        QMessageBox::StandardButton ans = QMessageBox::warning(this,tr("File not saved"),tr("Modifications are occuring on the configuration file. Do you want to save them?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if (ans == QMessageBox::Yes) save();
    }
    e->accept();
}

void ConfigFileWidget::closeWindow()
{
    if (edited)
    {
        QMessageBox::StandardButton ans = QMessageBox::warning(this,tr("File not saved"),tr("Modifications are occuring on the configuration file. Do you want to save them?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if (ans == QMessageBox::Yes) save();
    }
    accept();
}
