#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    display(xdr::xDisplay()),
    driver(xdr::xDriver())
{
    this->backup = xdr::xBackup();
    ui->setupUi(this);
    ui->downloadFinished->hide();
    ui->ProgressBar->hide();
    ui->ListResolution->clear();
    for (size_t i = 0; i < display.screenSizes[0].size(); ++i) {
        auto size = display.screenSizes[0][i];
        char c[32];
        sprintf(c, "%dx%d", size.width, size.height);
        QString text = QString(c);
        ui->ListResolution->addItem(text);
    }
    auto backups = backup.get_backup_list();
    QStringList list;
    for(size_t i = 0; i < backups.size(); ++i) {
        std::string name = backups[i];
        list.push_back(QString(name.c_str()));
    }
    QStringListModel* model = new QStringListModel(list);
    ui->backupList->setModel(model);
    ui->infoGpu->setText(QString(xdr::GetGraphicDeviceName().c_str()));
    ui->infoScreen->setText(QString(display.screenName.c_str()));
    XRRScreenSize size = display.getCurrentResolution(display.defaultScreen);
    std::string resolution = std::to_string(size.width) + 'x' + std::to_string(size.height);
    ui->infoResolution->setText(QString(resolution.c_str()));
    updateRates();
    auto drivers = driver.getVersions();
    QStringList driversList;
    for (const auto& var: drivers) {
        driversList.push_back(QString(var.c_str()));
    }
    driversList.push_back(QString("470.256.02"));
    QStringListModel* driversModel = new QStringListModel(driversList);
    ui->listDrivers->setModel(driversModel);
    ui->driverGPU->setText(ui->driverGPU->text() + '\t' + driver.graphicCardName);
    ui->driverCurrent->setText(ui->driverCurrent->text() + '\t' + driver.driverName);
    ui->driverVersion->setText(ui->driverVersion->text() + '\t' + driver.driverVersion);
}

void MainWindow::updateRates() {
    ui->listHZ->clear();
#if 0
    for (const auto& v: display.screenRates) {
        ui->listHZ->addItem(QString(std::to_string(v).c_str()));
    }
#else
    for (int i = 0; i < display.screenResources->nmode; ++i) {
        XRRModeInfo mode = display.screenResources->modes[i];
        auto curResolution = display.selectedScreenSize;
        if((curResolution.width != mode.width) || (curResolution.height != mode.height)) continue;
        double refresh = (double)mode.dotClock / ((double) mode.vTotal * (double) mode.hTotal);
        char c[16];
        sprintf(c, "%6.2f", refresh);
        ui->listHZ->addItem(QString(c));
    }
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Resolution_clicked()
{
    //ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setCurrentWidget(ui->pageResolution);
}

void MainWindow::on_Resolution_clicked(bool _clicked)
{

}

void MainWindow::on_Backup_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageBackups);
    auto backups = backup.get_backup_list();
    QStringList list;
    for(size_t i = 0; i < backups.size(); ++i) {
        std::string name = backups[i];
        list.push_back(QString(name.c_str()));
    }
    QStringListModel* model = new QStringListModel(list);
    ui->backupList->setModel(model);
}

void MainWindow::on_Drivers_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageDrivers);
}

void MainWindow::on_Download_clicked()
{
    QModelIndex index = ui->listDrivers->currentIndex();
    std::string version = index.data(Qt::DisplayRole).toString().toStdString();
    QString filepath = QString((std::string("/tmp/") + driver.getVersionFileName(version)).c_str());
    if (QFile(filepath).exists()) {
        return;
    }else {
        ui->downloadFinished->hide();
        download.download(QUrl(driver.getLinkToVersion(version).c_str()), QString("/tmp/"));
        ui->ProgressBar->show();
        connect(&download, SIGNAL(percentDownloaded(int)), ui->ProgressBar, SLOT(setValue(int)));
        connect(&download, SIGNAL(downloaded(QString)), ui->downloadFinished, SLOT(show()));
    }
}

void MainWindow::on_SetButton_clicked()
{
    int i = ui->ListResolution->currentIndex();
    short rate = display.screenRates[ui->listHZ->currentIndex()];
    Rotation rotation = 1 << (ui->listOrientation->currentIndex());
    display.ChangeCurrentResolutionRates(i, rate, rotation);
}

void MainWindow::on_Information_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageInformation);
}

void MainWindow::on_CreateBackupButton_clicked()
{
    backup.make_backup();
    auto backups = backup.get_backup_list();
    QStringList list;
    for(size_t i = 0; i < backups.size(); ++i) {
        std::string name = backups[i];
        list.push_back(QString(name.c_str()));
    }
    QStringListModel* model = new QStringListModel(list);
    ui->backupList->setModel(model);
}

void MainWindow::on_ListResolution_activated(int index)
{
    display.selectedScreenSizeId = index;
    display.selectedScreenSize = display.screenSizes[display.selectedScreenId][index];
    display.getSelectedRates();
    updateRates();
}

void MainWindow::on_Install_clicked()
{
    QModelIndex index = ui->listDrivers->currentIndex();
    std::string version = index.data(Qt::DisplayRole).toString().toStdString();
    QString filepath = QString((std::string("/tmp/") + driver.getVersionFileName(version)).c_str());
    if (QFile(filepath).exists()) {
        download.installFile(filepath);
    }else {
        return;
    }
}
