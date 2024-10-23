#include "monitorbutton.h"
#include "ui_monitorbutton.h"

MonitorButton::MonitorButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorButton)
{
    ui->setupUi(this);
}

MonitorButton::~MonitorButton()
{
    delete ui;
}
