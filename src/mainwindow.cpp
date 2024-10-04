#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->stat_list = new QListWidget();
    stat_list->addItem("Item 1");
    stat_list->addItem("Item 2");
    stat_list->addItem("Item 3");
}

MainWindow::~MainWindow()
{
    delete ui;
}
