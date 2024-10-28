#include "confirm.h"
#include "ui_confirm.h"

confirm::confirm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::confirm)
{
    ui->setupUi(this);
}

confirm::~confirm()
{
    delete ui;
}
