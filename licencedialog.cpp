#include "licencedialog.h"
#include "ui_licencedialog.h"

#include <QFile>
#include <QByteArray>
#include <QProcess>
#include <QSaveFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringBuilder>

LicenceDialog::LicenceDialog(LicenceDialogType wType, int windowWidth, int windowHeight, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenceDialog),
    exportButton(new QPushButton(tr("Export as...")))
{
    ui->setupUi(this);
    ui->buttonBox->addButton(exportButton,QDialogButtonBox::ActionRole);
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
    case Copy:
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
    default:
        setWindowTitle(tr("Network information"));
#ifdef _WIN32
        const QString networkQueryProcessName = "ipconfig";
#else
        const QString networkQueryProcessName = "ifconfig";
#endif
        QProcess networkQuery;
        networkQuery.setProgram(networkQueryProcessName);
        networkQuery.setProcessChannelMode(QProcess::MergedChannels);
        networkQuery.start();
        networkQuery.waitForFinished(5000);
        ui->textBrowser->setPlainText(networkQuery.readAll());
        break;
    }
    resize(windowWidth,windowHeight);
    connect(exportButton,SIGNAL(clicked()),this,SLOT(exportText()));
}

LicenceDialog::~LicenceDialog()
{
    delete exportButton;
    delete ui;
}

void LicenceDialog::exportText()
{
    const QString filename = QFileDialog::getSaveFileName(this,tr("Export information as..."),QDir::homePath(),tr("Text document (*.txt);;Markdown (*.md)"));
    if (filename.isEmpty()) return;
    QSaveFile file(filename);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::critical(this,tr("Unable to write file"),tr("File at location \"") % filename % tr("\" cannot be opened. Aborting export task."));
    }
    if (filename.endsWith(".md"))
    {
        file.write(ui->textBrowser->toMarkdown().toLocal8Bit());
    }
    else
    {
        file.write(ui->textBrowser->toPlainText().toLocal8Bit());
    }
    file.commit();
}
