#include "driverdialog.h"
#include "ui_driverdialog.h"

DriverDialog::DriverDialog(QWidget *parent, QString name) :
    QDialog(parent),
    ui(new Ui::DriverDialog)
{
    ui->setupUi(this);
    ui->label->setText(name);
}

DriverDialog::~DriverDialog()
{
    delete ui;
}
