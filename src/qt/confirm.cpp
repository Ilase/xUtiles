#include "confirm.h"
#include "ui_confirm.h"

Confirm::Confirm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Confirm)
{
    ui->setupUi(this);
    timer = new QTimer;
    timer->setSingleShot(true);
    timer->setInterval(15000);
    applyed = false;
    connect(timer, &QTimer::timeout, this, &Confirm::timerTimeout);
    connect(this, &Confirm::open, this, &Confirm::on_modalWindowOpen);
}

Confirm::~Confirm()
{
    delete ui;
    delete timer;
}


void Confirm::on_cancelButton_clicked()
{
    applyed = false;
    emit closed(applyed);
    timer->stop();
    close();

}

void Confirm::on_applyButton_clicked()
{
    applyed = true;
    //emit closed(applyed);
    emit applySignal();
    timer->stop();
    close();
}

void Confirm::on_modalWindowOpen()
{
    timer -> start();
}

void Confirm::timerTimeout()
{
    timer -> stop();
    emit cancelSignal();
    close();
}

void Confirm::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    deleteLater();
}
//void Confirm::on_Confirm_destroyed()
//{
//    applyed = true;
//}


