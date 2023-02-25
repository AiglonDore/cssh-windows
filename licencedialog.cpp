#include "licencedialog.h"
#include "ui_licencedialog.h"

#include <QFile>
#include <QByteArray>

LicenceDialog::LicenceDialog(LicenceDialogType wType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenceDialog)
{
    ui->setupUi(this);
    QFile licenceFile("LICENSE.md");
    QString fileContent(tr("File \"LICENSE.md\" not found."));
    if (licenceFile.open(QIODevice::ReadOnly))
    {
        fileContent = licenceFile.readAll();
        licenceFile.close();
    }

    switch (wType)
    {
    case Global:
        setWindowTitle(tr("License"));
        ui->textBrowser->setMarkdown(fileContent);
        break;
    case Warranty:
        setWindowTitle(tr("Warranty license"));
        ui->textBrowser->setMarkdown(tr("# DISCLAIMER OF WARRANTY\nTHERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY \
APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT \
HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY \
OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, \
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR \
PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM \
IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF \
ALL NECESSARY SERVICING, REPAIR OR CORRECTION.","Part of the GPL3 license"));
        break;
    default:
        setWindowTitle(tr("Liability license"));
        ui->textBrowser->setMarkdown(tr("# DISCLAIMER OF LIABILITY\nIN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING \
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS \
THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY \
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE \
USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF \
DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD \
PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), \
EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF \
SUCH DAMAGES.","Part of the GPL3 license"));
        break;
    }
}

LicenceDialog::~LicenceDialog()
{
    delete ui;
}
