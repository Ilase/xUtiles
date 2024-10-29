#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QStringListModel>
#include "xutiles-backup.hpp"
#include "xutiles-drivers.hpp"
#include "xutiles-display.hpp"
#include "xutiles-download.hpp"
#include "xutiles-xconfig.hpp"
#include "xutiles.hpp"
#include "ui_mainwindow.h"
#include <iostream>
#include <vector>
#include <QTimer>
#include "confirm.h"
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

signals:
    void timeoutSignal();


private slots:
    void on_Resolution_clicked();

    void on_Resolution_clicked(bool checked);

    void on_Backup_clicked();

    void on_Drivers_clicked();

    void on_Download_clicked();

    void on_SetButton_clicked();

    void on_Information_clicked();

    void on_CreateBackupButton_clicked();

    void on_ListResolution_activated(int index);

    void on_Install_clicked();

    void on_BackupButton_clicked();

    void on_additionalDriverSettings_clicked();

    void on_downloadRecomended_clicked();

    int on_modalWindowClosed(bool applyed);

    void on_dependsScreen_clicked();

    void on_exitButton_clicked();

    void on_checkBoxTearing_clicked();

    void on_timeout();


private:
    Ui::Dialog *ui;
    xdr::xDisplay display;
    xdr::xBackup backup;
    xdr::xDriver driver;
    std::vector<QLabel> backupsName;

    xdr::xDownload download;
    void updateRates();
    int openApplyWindow();
    QTimer *timer;
    Confirm *apply;
    int previousIndex;
};

#endif // MAINWINDOW_H
