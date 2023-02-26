#ifndef CONFIGFILEWIDGET_H
#define CONFIGFILEWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>

namespace Ui {
class ConfigFileWidget;
}

class ConfigFileWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigFileWidget(const QString& filename, int width, int height, QWidget *parent = nullptr);
    virtual ~ConfigFileWidget();

protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void closeEvent(QCloseEvent *e) override;

private slots:
    void textEdited();
    void save();
    void saveAs();
    void closeWindow();

private:
    Ui::ConfigFileWidget *ui;
    bool edited;
    QPushButton *saveButton;
    QPushButton *saveAsButton;
    QPushButton *discardButton;
    QPushButton *closeButton;
    QString fileLocation;
};

#endif // CONFIGFILEWIDGET_H
