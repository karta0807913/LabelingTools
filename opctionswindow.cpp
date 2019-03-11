#include <QFileInfo>
#include <QSettings>
#include <QFileDialog>

#include "opctionswindow.h"
#include "ui_opctionswindow.h"

OpctionsWindow::OpctionsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpctionsWindow)
{
    ui->setupUi(this);
    loadSetting();
    connect(ui->browseOutputPath, SIGNAL(clicked()), this, SLOT(browseOutputPath()));
}

void OpctionsWindow::updateShortcut()
{
    frameShortcut       = (ui->Frame_MainKey->currentText().isEmpty())  ? "" : ui->Frame_MainKey->currentText()     + "+";
    arrorShortcut       = (ui->Arrow_MainKey->currentText().isEmpty())  ? "" : ui->Arrow_MainKey->currentText()     + "+";
    scalingShortcut     = (ui->Scaling_MainKey->currentText().isEmpty())? "" : ui->Scaling_MainKey->currentText()   + "+";
    exportShortcut      = (ui->Export_MainKey->currentText().isEmpty()) ? "" : ui->Export_MainKey->currentText()    + "+";

    frameShortcut   += ui->Frame_Key->currentText();
    arrorShortcut   += ui->Arrow_Key->currentText();
    scalingShortcut += ui->Scaling_Key->currentText();
    exportShortcut  += ui->Export_Key->currentText();

    upShortcut = ui->Up_Key->currentText();
    downShortcut = ui->Down_Key->currentText();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(close()));
}

void OpctionsWindow::setRecallFunction(MainWindow *window)
{
    this->window = window;
}

QString OpctionsWindow::getOutPutPath()
{
    return ui->outputFilePath->text();
}

class MainWindow{public :void updateShortcut();};

bool OpctionsWindow::close()
{
    updateShortcut();
    if(window)
        window->updateShortcut();
    return QMainWindow::close();
}

void OpctionsWindow::loadSetting()
{
    QFileInfo file(SettingFilePath);
    if(!file.exists())
        return;
    QSettings settings(SettingFilePath, QSettings::IniFormat);

    /* Load Path */
    ui->outputFilePath->setText(settings.value("Output Path").toString());

    ui->Frame_MainKey->setCurrentIndex(settings.value("Frame Main Key").toInt());
    ui->Arrow_MainKey->setCurrentIndex(settings.value("Arror Main Key").toInt());
    ui->Scaling_MainKey->setCurrentIndex(settings.value("Scaling Main Key").toInt());
    ui->Export_MainKey->setCurrentIndex(settings.value("Export Main Key").toInt());

    ui->Frame_Key->setCurrentIndex(settings.value("Frame Main").toInt());
    ui->Arrow_Key->setCurrentIndex(settings.value("Arror Main").toInt());
    ui->Scaling_Key->setCurrentIndex(settings.value("Scaling Main").toInt());
    ui->Up_Key->setCurrentIndex(settings.value("Up Key").toInt());
    ui->Down_Key->setCurrentIndex(settings.value("Down Key").toInt());
    ui->Export_Key->setCurrentIndex(settings.value("Export Key").toInt());


    updateShortcut();

    /* End */
}

void OpctionsWindow::saveSetting()
{
    QSettings settings(SettingFilePath, QSettings::IniFormat);

    /* Save Path */
    settings.setValue("Output Path", ui->outputFilePath->text());

    settings.setValue("Frame Main Key",     ui->Frame_MainKey->currentIndex());
    settings.setValue("Arror Main Key",     ui->Arrow_MainKey->currentIndex());
    settings.setValue("Scaling Main Key",   ui->Scaling_MainKey->currentIndex());
    settings.setValue("Export Main Key",    ui->Export_MainKey->currentIndex());

    settings.setValue("Frame Main",         ui->Frame_Key->currentIndex());
    settings.setValue("Arror Main",         ui->Arrow_Key->currentIndex());
    settings.setValue("Scaling Main",       ui->Scaling_Key->currentIndex());
    settings.setValue("Up Key",             ui->Up_Key->currentIndex());
    settings.setValue("Down Key",           ui->Down_Key->currentIndex());
    settings.setValue("Export Key",         ui->Export_Key->currentIndex());
}

void OpctionsWindow::browseOutputPath()
{
    QString path = QFileDialog::getExistingDirectory(
                NULL,
                "Open Floder",
                ui->outputFilePath->text(),
                QFileDialog::ShowDirsOnly);
    if(path.isNull())
        return;
    ui->outputFilePath->setText(path);
}

OpctionsWindow::~OpctionsWindow()
{
    saveSetting();
    delete ui;
}
