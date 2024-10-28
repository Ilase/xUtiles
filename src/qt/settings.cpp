#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

}

void Settings::on_checkBoxTearing_clicked()
{
    xdr::change_tearing(ui->checkBoxTearing->isChecked(), display.screenName);
}

Settings::~Settings()
{
    delete ui;
}
