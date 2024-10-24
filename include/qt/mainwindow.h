#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QStringListModel>
#include "xutiles-backup.hpp"
#include "xutiles-drivers.hpp"
#include "xutiles-display.hpp"
#include "xutiles-download.hpp"
#include "xutiles.hpp"
#include "ui_mainwindow.h"
#include <iostream>
#include <vector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    Ui::Dialog *ui;
    xdr::xDisplay display;
    xdr::xBackup backup;
    xdr::xDriver driver;
    std::vector<QLabel> backupsName;

    xdr::xDownload download;
    void updateRates();
};

#endif // MAINWINDOW_H
