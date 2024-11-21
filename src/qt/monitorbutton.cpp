#include "monitorbutton.h"
#include "ui_monitorbutton.h"
#include <iostream>

MonitorButton::MonitorButton(QWidget *parent, int id, QString name) :
    QWidget(parent),
    ui(new Ui::MonitorButton),
    m_id(id),
    m_name(name)
{

    ui->setupUi(this);
    ui->monitorButton->setText(m_name);
}

MonitorButton::~MonitorButton()
{
    delete ui;
}

void MonitorButton::on_monitorButton_clicked()
{
    emit changedScreen(m_id);
}
