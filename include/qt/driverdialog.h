#ifndef DRIVERDIALOG_H
#define DRIVERDIALOG_H

#include <QDialog>

namespace Ui {
class DriverDialog;
}

class DriverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DriverDialog(QWidget *parent = nullptr, QString name = "");
    ~DriverDialog();

private:
    Ui::DriverDialog *ui;
};

#endif // DRIVERDIALOG_H
