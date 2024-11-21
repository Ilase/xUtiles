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
    ui->infoCore->setText(xdr::exec("uname -r").c_str());
    //INFO GRAPHICS CARD

    ui->infoGpu->setText(QString(xdr::GetGraphicDeviceName().c_str()));
    for (int i = 0; i < driver.graphicCardNames.size(); ++i) {
        ui->graphicDeviceSelect->addItem(driver.graphicCardNames[i]);
    }


    //RATES
    updateRates();
    //SCREEN INFO
    int gcd = std::gcd(display.selectedScreenSize.width, display.selectedScreenSize.height);
    char text[8];
    sprintf(text, "%d:%d", display.selectedScreenSize.width / gcd, display.selectedScreenSize.height / gcd);
    ui->displayFormat->setText(ui->displayFormat->text() +" "+ text);
    ui->displayName->setText(ui->displayName->text() + " " + display.screenName.c_str());
    char res[32];
    sprintf(res, "%dx%d", display.selectedScreenSize.width, display.selectedScreenSize.height);
    ui->displayResolution->setText(ui->displayResolution->text() + res);

    //MONITOR BUTTONS
    for (int i = 0; i < display.screenResources->noutput; i++){
        auto output = XRRGetOutputInfo(display.display, display.screenResources, display.screenResources->outputs[i]);
        if (output->connection !=0 ){
            continue;
        }
        for (int j = 0; j < output->nmode; ++j) {
        }
        auto button = new MonitorButton(this, i, output->name);
        ui->monitorList->addWidget(button);
        connect(button, SIGNAL(changedScreen(int)),this, SLOT(changeScreen(int)));
    }
    //MODULES TABLE
    QStringList headers = {"Название", "Размер", "Использован", "Кем"};
    ui->tableModules->setHorizontalHeaderLabels(headers);
    ui->tableModules->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QString modules = xdr::exec("lsmod").c_str();
    QRegularExpression modulesPatern(R"((\w+) +(\d+) +(\d+) ?+([\w,]+)?)");
    QRegularExpressionMatchIterator mathces = modulesPatern.globalMatch(modules);
    int i = 0;
    while (mathces.hasNext()) {
        QRegularExpressionMatch match = mathces.next();
        ui->tableModules->insertRow(i);
        if (match.hasMatch()) {
            for (int u = 1; u < 5; ++u) {
                ui->tableModules->setItem(i, u - 1, new QTableWidgetItem(match.captured(u)));
            }
        }
        i++;
    }
    //DEBUG
#ifdef QT_DEBUG
    ui->debugCardName->setEnabled(true);
    ui->debugCardSearch->setEnabled(true);
    ui->debugCardName->setVisible(true);
    ui->debugCardSearch->setVisible(true);
#else
    ui->debugCardName->setVisible(false);
    ui->debugCardSearch->setVisible(false);
#endif
    ui->displayResolution->setText(ui->displayResolution->text() + " " + res);
    connect(ui->ListResolution, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::on_ListResolution_activated);

    //MODAL WINDOW
    previousIndex = ui->ListResolution->currentIndex();
    initializing = false;

    //REMOVE RESOLUTION
    auto addedResolution = xdr::exec(" cat ~/.xsessionrc | grep '# ' |sed 's/# //' | sed 's/ Hz.*//'");
    std::string line;
    auto stream = std::istringstream(addedResolution);
    while (getline(stream, line)) {
        ui->addedResolutionList->addItem(line.c_str());
    }

    //MODPROBE FILES

    for (const auto & entry : fs::directory_iterator("/etc/modprobe.d")) {
        ui->modprobeFileList->addItem(QString(entry.path().c_str()));
        QFile inputFile(entry.path().c_str());
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           QString text;
           while (!in.atEnd())
           {
              QString line = in.readLine();
              text += line + '\n';
           }
           inputFile.close();
           modprobeFilesBuffer.push_back(text);
        }
    }
    ui->textEditModprobe->setText(modprobeFilesBuffer[0]);
#if 0
    char* buffer;
    for (const auto & entry : fs::directory_iterator("/etc/modprobe.d")) {
        ui->modprobeFileList->addItem(QString(entry.path().c_str()));
        std::ifstream file(entry.path());
        int length;
        file.seekg(0, std::ios::end);
        length = (int)file.tellg() - 1;
        buffer = new char[length];
        file.seekg(0, std::ios::beg);
        file.read(buffer, length);
        std::cout << entry.path() << '\t' << length  << '\n' << buffer << '\n';
        file.close();
        modprobeFilesBuffer.push_back(buffer);
        delete[] buffer;
    }
#endif
}

void MainWindow::on_pushButtonTest_clicked() {

}

void MainWindow::changeScreen(int id){
    display.changeScreen(id);
    updateResolutions();
    updateRates();
}

void MainWindow::updateRates() {
    ui->listHZ->clear();
    auto output = XRRGetOutputInfo(display.display, display.screenResources, display.screenResources->outputs[display.selectedScreenId]);
    for (int i = 0; i < display.screenResources->nmode; ++i) {
        XRRModeInfo mode = display.screenResources->modes[i];
        bool contains = false;
        for (int j = 0; j < output->nmode; ++j) {
            if(output->modes[j] == mode.id){
                contains = true;
            }
        }
        if (!contains) {
            continue;
        }
        auto curResolution = display.selectedScreenSize;
        if((curResolution.width != mode.width) || (curResolution.height != mode.height)) continue;
        double refresh = (double)mode.dotClock / ((double) mode.vTotal * (double) mode.hTotal);
        char c[16];
        sprintf(c, "%6.2f", refresh);
        ui->listHZ->addItem(QString(c));
    }
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

    int i = ui->graphicDeviceSelect->currentIndex();
    ui->driverGPU->setText(QString("Видеокарта: %1").arg(driver.graphicCardNames[i]));
    ui->driverCurrent->setText(QString("Драйвер: %1").arg(driver.driverNames[i]));
    ui->driverVersion->setText(QString("Версия: %1").arg(driver.driverVersions[i]));
    std::string name = driver.graphicCardNames[i].toStdString();
    if (name.find('[') != std::string::npos) {
        std::cout << name.find('[') << '\n';
        QRegularExpression r(R"(\[(\w+(?: \w+)+)\])");
        QRegularExpressionMatch m = r.match(driver.graphicCardNames[i]);
        name = m.captured(1).toStdString();
    }
    auto drivers = driver.getDrivers(name);
    if (drivers.size() == 0) {
        QDialog *di = new DriverDialog(this, QString("Не найдено дополнительных драйверов для видеоадаптера %1").arg(name.c_str()));
        di->show();
        return;
    }else if (std::stoi(drivers[0].version.substr(0, drivers[0].version.find('.'))) < 470) {
        QDialog *di = new DriverDialog(this, QString("Драйверы для видеоадаптара %1 устарели, установока драйверов будет невозможной").arg(name.c_str()));
        di->show();
    }
    QStringList driversList;
    for (const auto& var: drivers) {
        auto version = var.version;
        QString filepath = QString((xdr::driverFolderName() + driver.getVersionFileName(version)).c_str());
        if (QFile(filepath).exists()) {
            driversList.push_back(QString((version + "*").c_str()));
        }else {
            driversList.push_back(QString(version.c_str()));
        }
    }
    QStringListModel* driversModel = new QStringListModel(driversList);
    ui->listDrivers->setModel(driversModel);
    ui->stackedWidget->setCurrentWidget(ui->pageInstallDrivers);

}

void MainWindow::on_downloadRecomended_clicked()
{
    QString devicename;
#ifdef QT_DEBUG
    if (!ui->debugCardName->toPlainText().isEmpty()) {
        devicename = ui->debugCardName->toPlainText();
    }else {
        int i = ui->graphicDeviceSelect->currentIndex();
        devicename = driver.graphicCardNames[i];
    }
#else
    int i = ui->graphicDeviceSelect->currentIndex();
    devicename = driver.graphicCardNames[i];

#endif
    if (devicename.contains("NVIDIA")) {
        QFile supported_gpus;
        supported_gpus.setFileName("/opt/xUtils/supported-gpus.json");
        supported_gpus.open(QIODevice::ReadOnly | QIODevice::Text);
        QJsonDocument doc = QJsonDocument::fromJson(supported_gpus.readAll());
        auto cards = doc.object()["chips"].toArray();
#ifdef QT_DEBUG
        auto cardPCI = "128B";
#else
        auto cardPCI = QString(xdr::exec(R"(lspci -nn | grep -E "VGA|3D" | sed -n 's/.*\[.*:\(.*\)\].*/\1/p')").c_str()).trimmed();
#endif
        for (const auto& card: cards) {
            QJsonObject cardobj = card.toObject();
            qDebug() << cardobj["devid"].toString().toUpper() << '\t' << cardPCI.toUpper();
            if (cardobj["devid"].toString().toUpper().contains(cardPCI.toUpper())) {
                auto legacybranch = cardobj["legacybranch"];
                if(legacybranch != QJsonValue::Null){
                    if (legacybranch.toString() == "470.xx"){
                        system("systemd-run apt install -y nvidia-tesla-470-driver");
                    }else {
                        QMessageBox(QMessageBox::Critical, tr("Ошибка установки драйвера"), tr("Видеокарта не подерживается"), QMessageBox::Close, this).exec();
                    }
                }else {
                    system("systemd-run apt install -y nvidia-driver");
                }
                break;
            }
        }
    }else if (devicename.contains("AMD")) {
        system("systemd-run apt install -y xserver-xorg-video-amdgpu");
    }else if (devicename.contains("Intel")) {
        QDialog *di = new DriverDialog(this, QString("Не найдено рекомендованных драйверов для видеоадаптера %1").arg(devicename));
        di->show();
    }else {
        QDialog *di = new DriverDialog(this, QString("Не найдено рекомендованных драйверов для видеоадаптера %1").arg(devicename));
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
    //xdr::change_tearing(ui->checkBoxTearing->isChecked(), display.screenName);
}

void MainWindow::setNewMonitorResolution(int index, short rate)
{

    qDebug() << "new resolution" << ui->ListResolution->currentText();
    Rotation rotation = 1 << (ui->listOrientation->currentIndex());
    display.ChangeCurrentResolutionRates(index, rate, rotation);
    //ui->pageResolution->update();
}

void MainWindow::on_ListResolution_currentIndexChanged(int index)
{
#if 0
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
#endif
}

void MainWindow::onCancel(bool activated)
{
    setNewMonitorResolution(display.previousScreenSizeId, display.previousRate);
}



void MainWindow::previousIndexChange()
{
    previousIndex = ui->ListResolution->currentIndex();

    display.previousRate =  display.screenRates[ui->listHZ->currentIndex()];
    display.previousScreenSizeId =  ui->ListResolution->currentIndex();
}

void MainWindow::on_debugCardSearch_clicked()
{
    QString cardName = ui->debugCardName->toPlainText();
    //int i = ui->graphicDeviceSelect->currentIndex();
    ui->driverGPU->setText(QString("Видеокарта: %1").arg(cardName));
    ui->driverCurrent->setText(QString("Драйвер: %1").arg(""));
    ui->driverVersion->setText(QString("Версия: %1").arg(""));
    std::string name = cardName.toStdString();
    if (name.find('[') != std::string::npos) {
        std::cout << name.find('[') << '\n';
        QRegularExpression r(R"(\[(\w+(?: \w+)+)\])");
        QRegularExpressionMatch m = r.match(cardName);
        name = m.captured(1).toStdString();
    }
    auto drivers = driver.getDrivers(name);
    if (drivers.size() == 0) {
        QDialog *di = new DriverDialog(this, QString("Не найдено дополнительных драйверов для видеоадаптера %1").arg(name.c_str()));
        di->show();
        return;
    }
    QStringList driversList;
    for (const auto& var: drivers) {
        auto version = var.version;
        QString filepath = QString((xdr::driverFolderName() + driver.getVersionFileName(version)).c_str());
        if (QFile(filepath).exists()) {
            driversList.push_back(QString((version + "*").c_str()));
        }else {
            driversList.push_back(QString(version.c_str()));
        }
    }
    QStringListModel* driversModel = new QStringListModel(driversList);
    ui->listDrivers->setModel(driversModel);

    ui->stackedWidget->setCurrentWidget(ui->pageInstallDrivers);
}

void MainWindow::updateResolutions() {
    display.getResolutions();
    ui->ListResolution->clear();
    for (size_t i = 0; i < display.screenSizes[0].size(); ++i) {
        auto size = display.screenSizes[0][i];
        char c[32];
        sprintf(c, "%dx%d", size.width, size.height);
        QString text = QString(c);
        ui->ListResolution->addItem(text);
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageInstallDrivers);
}

void MainWindow::on_CoreModules_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageModprobe);
}

void MainWindow::on_apply_clicked()
{
    int i = ui->graphicDeviceSelect->currentIndex();
    QString devicename = driver.graphicCardNames[i];
    if (devicename.contains("NVIDIA")) {
        system("systemd-run sh /opt/xUtils/bin/xorg-parser nvidia Card0");
    }else if (devicename.contains("AMD")) {
        system("systemd-run sh /opt/xUtils/bin/xorg-parser amdgpu Card0");
    }else if (devicename.contains("Intel")) {
        system("systemd-run sh /opt/xUtils/bin/xorg-parser intel Card0");
    }else {
        QDialog *di = new DriverDialog(this, QString("Невозможно применить настройки для видеоадаптера %1").arg(devicename));
        di->show();
    }
}

void MainWindow::on_buttonAddResolution_clicked()
{
    bool read = true;
    bool check = false;
    int width = ui->addResolutionWidth->toPlainText().toInt(&check);
    read &= check;
    int height = ui->addResolutionHeight->toPlainText().toInt(&check);
    read &= check;
    int rate = ui->addResolutionRate->toPlainText().toInt(&check);
    read &= check;
    if (!read) {
        QDialog *error = new DriverDialog(this, "Некоректное разрешение экрана");
        error->show();
        return;
    }else {
        if (display.addResolution(width, height, rate) != 0) {
            QDialog *error = new DriverDialog(this, "Не удалось добавить разрешение");
            error->show();
            return;
        }else{
            updateResolutions();
            updateRates();
            auto addedResolution = xdr::exec(" cat ~/.xsessionrc | grep '# ' |sed 's/# //' | sed 's/ Hz.*//'");
            std::string line;
            auto stream = std::istringstream(addedResolution);
            ui->addedResolutionList->clear();
            while (getline(stream, line)) {
                ui->addedResolutionList->addItem(line.c_str());
            }
        }
    }
}

void MainWindow::on_applyButton_clicked()
{
    short rate = display.screenRates[ui->listHZ->currentIndex()];
    int index = ui->ListResolution->currentIndex();
    setNewMonitorResolution(index, rate);
    if(initializing) return;
    if(!(previousIndex == index && display.previousRate == rate))
    {
        apply = new Confirm;
        emit apply->open();
        connect(apply, &Confirm::closed, this, &MainWindow::onCancel);
        connect(apply, &Confirm::applySignal, this, &MainWindow::previousIndexChange);
        apply->show();
    }
    //ui->pageResolution->update();
}

void MainWindow::on_PCI_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pagePCI);
}

void MainWindow::on_buttonRemoveResolution_clicked()
{
    QString resolution = ui->addedResolutionList->currentText();
    if (resolution.size() < 1) {
        return;
    }
    std::string del = resolution.toStdString();
    std::string delcommand = xdr::exec(("sed -n '/" + del + "/{n;n;p;}' ~/.xsessionrc | sed 's/addmode/delmode/'").c_str());
    std::string del1command = xdr::exec(("sed -n '/" + del + "/{n;p;}' ~/.xsessionrc | grep -E -o 'xrandr --newmode \".*\"' | sed 's/newmode/rmmode/'").c_str());
    std::string command = "sed -i -e '/# " + resolution.toStdString() + "/,+2d' ~/.xsessionrc";
    system(command.c_str());
    ui->addedResolutionList->removeItem(ui->addedResolutionList->currentIndex());
    system(delcommand.c_str());
    system(del1command.c_str());
    updateResolutions();
    updateRates();
}

int MainWindow::writeToProtectedFile(const QString &path, const QString &content) {
    QDBusConnection bus = QDBusConnection::systemBus();
   if (!bus.isConnected())
   {
       QMessageBox(QMessageBox::Critical, tr("D-Bus error"), tr("Cannot connect to the D-Bus session bus."), QMessageBox::Close, this).exec();
       return 1;
   }

   // this is our Special Action that after allowed will call the helper
   QDBusMessage message = QDBusMessage::createMethodCall("org.qt.policykit.rootwrite", "/", "org.qt.policykit.rootwrite", QLatin1String("write"));

   // If a method in a helper file has arguments, enter the arguments.
   QList<QVariant> ArgsToHelper;
   ArgsToHelper << QVariant::fromValue(path) << QVariant::fromValue(content);
   message.setArguments(ArgsToHelper);

   // Send a message to DBus. (Execute the helper file.)
   QDBusMessage reply = QDBusConnection::systemBus().call(message);

   // Receive the return value (including arguments) from the helper file.
   // The methods in the helper file have two arguments, so check them.

   if (reply.type() == QDBusMessage::ReplyMessage && reply.arguments().size() == 2)
   {
       // the reply can be anything, here we receive a bool
       if (reply.arguments().at(0).toInt() == 0)
       {   // If the helper file method completes successfully after successful authentication
           QMessageBox(QMessageBox::NoIcon, tr("Successed"), tr("The file was written successfully."), QMessageBox::Close, this).exec();
           return 0;
       }
       else if (reply.arguments().at(0).toInt() == -1)
       {   // If the helper file method fails after successful authentication
           QString strErrMsg = reply.arguments().at(1).toString();
           QMessageBox(QMessageBox::Critical, tr("Failed"), tr("Failed to write file.\n") + strErrMsg, QMessageBox::Close, this).exec();
           return 1;
       }
       else
       {   // If the authentication is canceled
           QMessageBox(QMessageBox::NoIcon, tr("Cancelled"), tr("Writing of the file was canceled."), QMessageBox::Close, this).exec();
           return 1;
       }
   }
   else if (reply.type() == QDBusMessage::MethodCallMessage)
   {
       QMessageBox(QMessageBox::Warning, tr("Time out"), tr("Message did not receive a reply (timeout by message bus)."), QMessageBox::Close, this).exec();
       return 1;
   }
   else if (reply.type() == QDBusMessage::ErrorMessage)
   {
       std::cout << reply.errorMessage().toStdString();
       QMessageBox(QMessageBox::Critical, tr("D-Bus error"), tr(("Could not send message to D-Bus.\n" + reply.errorMessage().toStdString()).c_str()), QMessageBox::Close, this).exec();
       return 1;
   }

}

void MainWindow::on_modprobeFileList_activated(const QString &arg1)
{

}

void MainWindow::on_modprobeFileList_activated(int index)
{
    ui->textEditModprobe->setText(modprobeFilesBuffer[index]);
}

void MainWindow::on_textEditModprobe_textChanged()
{
    int index = ui->modprobeFileList->currentIndex();
    modprobeFilesBuffer[index] = ui->textEditModprobe->toPlainText();
}

void MainWindow::on_saveModprobeFile_clicked()
{
    QString filename = ui->modprobeFileList->currentText();
    QString content = ui->textEditModprobe->toPlainText();
    std::cout<< filename.toStdString() << '\n';
    writeToProtectedFile(filename, content);
}

void MainWindow::on_createModprobeFile_clicked()
{
    bool ok;
    QString filename = QInputDialog::getText(this, "Назвние файла","Введите название файла:", QLineEdit::Normal, "", &ok);
    if (!ok) {
        return;
    }
    if (filename.isEmpty()) {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не указанно имя файла"), QMessageBox::Close, this).exec();
        return;
    }
    if (!filename.isEmpty()) {
        QRegularExpression reg(R"(^[\w\.]*\.conf$)");
        if (!reg.match(filename).hasMatch()) {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Указано неверное имя файла"), QMessageBox::Close, this).exec();
            return;
        }
        ui->modprobeFileList->addItem("/etc/modprobe.d/" + filename);
        modprobeFilesBuffer.push_back("");
        ui->modprobeFileList->setCurrentIndex(ui->modprobeFileList->count() - 1);
        ui->textEditModprobe->setText(modprobeFilesBuffer[ui->modprobeFileList->count() - 1]);
    }
}
