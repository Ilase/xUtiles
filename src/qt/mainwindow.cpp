#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirm.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    display(xdr::xDisplay()),
    driver(xdr::xDriver())
{
    this->backup = xdr::xBackup();
    ui->setupUi(this);
    ui->downloadFinished->hide();
    ui->settings->setVisible(false);
    ui->ProgressBar->hide();
    ui->ListResolution->clear();
    ui->stackedWidget->setCurrentIndex(0);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->informationButton);
    buttonGroup->addButton(ui->Drivers);
    buttonGroup->addButton(ui->Resolution);
    buttonGroup->addButton(ui->Backup);
    buttonGroup->setExclusive(false);

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
    ui->backupList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listDrivers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //INFO
    ui->infoScreen->setText(QString(display.screenName.c_str()));
    XRRScreenSize size = display.getCurrentResolution(display.defaultScreen);
    std::string resolution = std::to_string(size.width) + 'x' + std::to_string(size.height);
    ui->infoResolution->setText(QString(resolution.c_str()));
    QString output = xdr::exec("inxi").c_str();
    QRegExp cpuReg(R"(CPU: (\w+(?: [\w\[\]()_-]+)+) \w+:)");
    QRegExp storageReg(R"(Storage: ((?:[\w\[\]%()._-]+ )+)\w+:)");
    QRegExp memReg(R"(Mem: ((?:[\w\[\]%()\/._-]+ )+)\w+:)");
    if (output.contains(cpuReg)) {
        ui->infoProccesor->setText(cpuReg.cap(1));
    }
    if (output.contains(storageReg)) {
        ui->infoStorage->setText(storageReg.cap(1));
    }
    if (output.contains(memReg)) {
        ui->infoMemory->setText(memReg.cap(1));
    }

    //INFO GRAPHICS CARD

    ui->infoGpu->setText(QString(xdr::GetGraphicDeviceName().c_str()));
    for (int i = 0; i < driver.graphicCardNames.size(); ++i) {
        ui->graphicDeviceSelect->addItem(driver.graphicCardNames[i]);
    }

    //RATES
    updateRates();

    //DRIVERS

    /*-------ВСТАВИТЬ В ФУНКИЮ ПЕРЕХОДА-------
    ui->driverGPU->setText(ui->driverGPU->text() + '\t' + driver.graphicCardNames[0]);
    ui->driverCurrent->setText(ui->driverCurrent->text() + '\t' + driver.driverName);
    ui->driverVersion->setText(ui->driverVersion->text() + '\t' + driver.driverVersion);
    ----------------------------------------*/
    //SCREEN INFO
    int gcd = std::gcd(display.selectedScreenSize.width, display.selectedScreenSize.height);
    char text[8];
    sprintf(text, "%d:%d", display.selectedScreenSize.width / gcd, display.selectedScreenSize.height / gcd);
    ui->displayFormat->setText(ui->displayFormat->text() +" "+ text);
    ui->displayName->setText(ui->displayName->text() + " " + display.screenName.c_str());
    char res[32];
    sprintf(res, "%dx%d", display.selectedScreenSize.width, display.selectedScreenSize.height);
    ui->displayResolution->setText(ui->displayResolution->text() + " " + res);
    connect(ui->ListResolution, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::on_ListResolution_activated);

    //MODAL WINDOW
    previousIndex = ui->ListResolution->currentIndex();
    initializing = false;

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
    ui->stackedWidget->setCurrentWidget(ui->pageResolution);
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
    QString filepath = QString((xdr::driverFolderName() + driver.getVersionFileName(version)).c_str());
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

//void MainWindow::on_SetButton_clicked()
//{
//    int i = ui->ListResolution->currentIndex();
//    short rate = display.screenRates[ui->listHZ->currentIndex()];
//    Rotation rotation = 1 << (ui->listOrientation->currentIndex());
//    display.ChangeCurrentResolutionRates(i, rate, rotation);
//}

void MainWindow::on_informationButton_clicked()
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

void MainWindow::on_BackupButton_clicked()
{
    QModelIndex index = ui->backupList->currentIndex();
    std::string version = index.data(Qt::DisplayRole).toString().toStdString();
    auto path = fs::u8path(version);
    xdr::repair_backup(path);
}

void MainWindow::on_additionalDriverSettings_clicked()
{

//    int i = ui->graphicDeviceSelect->currentIndex();
//    ui->currentGpu->setText(ui->currentGpu->text() + '\t' + driver.graphicCardNames[i]);
//    ui->currentDriver->setText(ui->currentDriver->text() + '\t' + driver.driverNames[i]);
//    ui->currentVersion->setText(ui->currentVersion->text() + '\t' + driver.driverVersions[i]);
//    std::string name;
//    if (driver.graphicCardNames[i].toStdString().find('[')) {
//        QRegularExpression r(R"(\[(\w+(?: \w+)+)\])");
//        QRegularExpressionMatch m = r.match(driver.graphicCardNames[i]);
//        name = m.captured(1).toStdString();
//    }else {
//        name = driver.graphicCardNames[i].toStdString();
//    }
//   // name = "GeForce GTX 1060 6GB";
//    auto drivers = driver.getDrivers(name);
//    QStringList driversList;
//    for (const auto& var: drivers) {
//        auto version = var.version;
//        QString filepath = QString((xdr::driverFolderName() + driver.getVersionFileName(version)).c_str());
//        if (QFile(filepath).exists()) {
//            driversList.push_back(QString((version + "*").c_str()));
//        }else {
//            driversList.push_back(QString(version.c_str()));
//        }
//    }
   // QStringListModel* driversModel = new QStringListModel(driversList);
    //ui->listDrivers->setModel(driversModel);
    ui->stackedWidget->setCurrentWidget(ui->pageInstallDrivers);

}

void MainWindow::on_downloadRecomended_clicked()
{
    int i = ui->graphicDeviceSelect->currentIndex();
    auto devicename = driver.graphicCardNames[i];
    if (devicename.contains("NVIDIA")) {
        system("systemd-run apt install ");
    }else if (devicename.contains("AMD")) {
        system("systemd-run apt install ");
    }else if (devicename.contains("Intel")) {
        QDialog *di = new QDialog(this);
        di->show();
    }else {
        QDialog *di = new QDialog(this);
        di->show();
    }
}

void MainWindow::on_dependsScreen_clicked()
{
    ui->settings->setVisible(true);
}

void MainWindow::on_exitButton_clicked()
{
    ui->settings->setVisible(false);
}

void MainWindow::on_checkBoxTearing_clicked()
{
    xdr::change_tearing(ui->checkBoxTearing->isChecked(), display.screenName);
}

void MainWindow::setNewMonitorResolution(int index)
{
    qDebug() << "new resolution" << ui->ListResolution->currentText();
    short rate = display.screenRates[ui->listHZ->currentIndex()];
    Rotation rotation = 1 << (ui->listOrientation->currentIndex());
    display.ChangeCurrentResolutionRates(index, rate, rotation);
    //ui->pageResolution->update();
}

void MainWindow::on_ListResolution_currentIndexChanged(int index)
{
    setNewMonitorResolution(index);
    if(initializing) return;
    if(previousIndex != index)
    {
        apply = new Confirm;
        emit apply->open();
        connect(apply, &Confirm::closed, this, &MainWindow::onCancel);
        connect(apply, &Confirm::applySignal, this, &MainWindow::previousIndexChange);
        apply->show();
    }
}

void MainWindow::onCancel(bool activated)
{
    setNewMonitorResolution(previousIndex);
}

void MainWindow::previousIndexChange()
{
    previousIndex = ui->ListResolution->currentIndex();
}


