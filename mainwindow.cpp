#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("sniffer-2022E8013382024-周淞杰");
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

