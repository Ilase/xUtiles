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
    notAplly = false;
    connect(timer, &QTimer::timeout, this, &Confirm::timerTimeout);
    connect(this, &Confirm::open, this, &Confirm::on_modalWindowOpen);
    setWindowFlags(Qt::Window);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

Confirm::~Confirm()
{
    delete ui;
    delete timer;
}


void Confirm::on_cancelButton_clicked()
{
    notAplly = true;
    emit closed(notAplly);
    timer->stop();
    close();

}

void Confirm::on_applyButton_clicked()
{
    notAplly = false;
    //emit closed(applyed);
    emit applySignal(notAplly);
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
    notAplly = true;
    emit closed(notAplly);
    close();
}

void Confirm::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    deleteLater();
}


