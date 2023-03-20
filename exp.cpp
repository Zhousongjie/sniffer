#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QFile>
#include <QDir>
#include <QTextStream>


void MainWindow::init()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, ipAddressesList)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ui->comboBox_exp_localIp->addItem(address.toString());
        }
    }

    dataList.clear();

    mySniffer = new MySniffer(this,ui->comboBox_exp_localIp->currentText(),0);
    connect(mySniffer,&MySniffer::finished,this,&MainWindow::exp_mySnifferFinish);
    connect(mySniffer,&MySniffer::sendData,this,&MainWindow::exp_recvSnifferData);
    connect(mySniffer,&MySniffer::sendError,this,&MainWindow::exp_mySnifferError);
    connect(this,&MainWindow::stopSnifferThread,mySniffer,&MySniffer::stopThread);

    ui->tableWidget_exp_result->setColumnCount(5);
    ui->tableWidget_exp_result->setHorizontalHeaderLabels(QStringList()<<"类型"<<"From"<<"To"<<"内容"<< "注释");
    ui->tableWidget_exp_result->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget_exp_result->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableWidget_exp_result->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableWidget_exp_result->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableWidget_exp_result->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);

}

void MainWindow::exp_mySnifferError(QString data)
{
    QMessageBox::critical(this,"Sniffer错误",data);
}

void MainWindow::exp_mySnifferFinish()
{
    ui->pushButton_exp_start->setText("开始");
    ui->comboBox_exp_localIp->setEnabled(true);
    ui->lineEdit_exp_Count->setEnabled(true);
}

void MainWindow::exp_recvSnifferData(QStringList data)
{

    if(ui->pushButton_exp_start->text() == "开始")
        return ;

    this->dataList<<data;

    ui->tableWidget_exp_result->setRowCount(dataList.size());
    for(int i=0;i<dataList.size();i++)
    {
        ui->tableWidget_exp_result->setItem(i,0,new QTableWidgetItem(dataList[i][0]));
        ui->tableWidget_exp_result->setItem(i,1,new QTableWidgetItem(dataList[i][1]));
        ui->tableWidget_exp_result->setItem(i,2,new QTableWidgetItem(dataList[i][2]));
        ui->tableWidget_exp_result->setItem(i,3,new QTableWidgetItem(dataList[i][3]));
        ui->tableWidget_exp_result->setItem(i,4,new QTableWidgetItem(dataList[i][4]));
        ui->tableWidget_exp_result->scrollToBottom();
    }
}

void MainWindow::on_pushButton_exp_start_clicked()
{
    if(ui->pushButton_exp_start->text() == "开始")
    {
        this->dataList.clear();

        mySniffer->setCount(ui->lineEdit_exp_Count->text().toInt());
        mySniffer->setIp(ui->comboBox_exp_localIp->currentText());

        mySniffer->start();

        ui->pushButton_exp_start->setText("暂停");
        ui->comboBox_exp_localIp->setEnabled(false);
        ui->lineEdit_exp_Count->setEnabled(false);

    }
    else if(ui->pushButton_exp_start->text() == "暂停")
    {
        emit this->stopSnifferThread();
        ui->pushButton_exp_start->setText("开始");
        ui->comboBox_exp_localIp->setEnabled(true);
        ui->lineEdit_exp_Count->setEnabled(true);
    }


}

void MainWindow::on_pushButton_exp_saveToFile_clicked()
{
    QFile file(QDir::currentPath()+"/SnifferData.txt");
    if(!file.open(QIODevice::Append))
    {
        QMessageBox::critical(this,"保存失败","文件保存失败，请检查权限或者文件是否被占用");
        return;
    }
    QTextStream fileOut(&file);
    foreach (QStringList t, dataList) {
        for(int i=0;i<t.size();i++)
        {
            fileOut<<t[i]<<"\r\n";
        }
        fileOut<<endl;
    }
    QMessageBox::information(this,"保存成功","数据保存在当前路径下:"+QDir::currentPath()+"/SnifferData.txt");
}
