#include "confirm.h"
#include "ui_confirm.h"

Confirm::Confirm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Confirm)
{
    ui->setupUi(this);
//    QTimer timer;
//    timer.setSingleShot(true);
//    timer.setInterval(15000);

//    timer.start()
    applyed = false;
}

Confirm::~Confirm()
{
    delete ui;
}


void Confirm::on_cancelButton_clicked()
{
    applyed = false;
    emit closed(applyed);
    close();
}

void Confirm::on_applyButton_clicked()
{
    applyed = true;
    emit closed(applyed);
    close();
}
