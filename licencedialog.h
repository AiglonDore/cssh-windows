#ifndef LICENCEDIALOG_H
#define LICENCEDIALOG_H

#include <QDialog>

enum LicenceDialogType {Global, Warranty, Copy};

namespace Ui {
class LicenceDialog;
}

class LicenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenceDialog(LicenceDialogType wType, QWidget *parent = nullptr);
    ~LicenceDialog();

private:
    Ui::LicenceDialog *ui;
};

#endif // LICENCEDIALOG_H
