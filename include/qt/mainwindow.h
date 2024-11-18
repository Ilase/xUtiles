#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QStringListModel>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QMessageBox>
#include "xutiles-backup.hpp"
#include "xutiles-drivers.hpp"
#include "xutiles-display.hpp"
#include "xutiles-download.hpp"
#include "xutiles-xconfig.hpp"
#include "xutiles.hpp"
#include "ui_mainwindow.h"
#include "driverdialog.h"
#include <iostream>
#include <vector>
#include <QTimer>
#include <QButtonGroup>
#include "confirm.h"
#include "driverdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //xdr::xDisplay getDisplay();
    int getPreviousIndex();
    int writeToProtectedFile(const QString&,const QString&);

signals:
    void timeoutSignal();


private slots:
    void on_Resolution_clicked();

    void on_Backup_clicked();

    void on_Drivers_clicked();

    void on_Download_clicked();

    void previousIndexChange();

    void on_informationButton_clicked();

    void on_CreateBackupButton_clicked();

    void on_ListResolution_activated(int index);

    void on_Install_clicked();

    void on_BackupButton_clicked();

    void on_additionalDriverSettings_clicked();

    void on_downloadRecomended_clicked();

    void on_dependsScreen_clicked();

    void on_exitButton_clicked();

    void on_checkBoxTearing_clicked();

    void on_ListResolution_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_debugCardSearch_clicked();
    void on_CoreModules_clicked();

    void on_apply_clicked();

    void on_buttonAddResolution_clicked();

    void on_applyButton_clicked();

    void on_PCI_clicked();

    void on_buttonRemoveResolution_clicked();

    void on_pushButtonTest_clicked();

    void on_modprobeFileList_activated(const QString &arg1);

    void on_modprobeFileList_activated(int index);

    void on_textEditModprobe_textChanged();

private:
    Ui::Dialog *ui;
    xdr::xDisplay display;
    xdr::xBackup backup;
    xdr::xDriver driver;
    std::vector<QLabel> backupsName;
    std::vector<QString> modprobeFilesBuffer;
    xdr::xDownload download;
    void updateRates();
    //int openApplyWindow();
    QTimer *timer;
    Confirm *apply;
    int previousIndex;
    bool initializing;
    QButtonGroup *buttonGroup;

    void setNewMonitorResolution(int , short);
    void onCancel(bool activated);
    void updateResolutions();
};

#endif // MAINWINDOW_H
