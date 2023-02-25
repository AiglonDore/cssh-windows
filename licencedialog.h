#ifndef LICENCEDIALOG_H
#define LICENCEDIALOG_H

#include <QDialog>
#include <QPushButton>

enum LicenceDialogType {Global, Warranty, Copy, Network};

namespace Ui {
class LicenceDialog;
}

class LicenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenceDialog(LicenceDialogType wType, int windowWidth, int windowHeight, QWidget *parent = nullptr);
    ~LicenceDialog();
private slots:
    void exportText();

private:
    Ui::LicenceDialog *ui;
    QPushButton *exportButton;
};

#endif // LICENCEDIALOG_H
