#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "mysniffer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QString DEFAULT_IP = QString("39.101.201.13");

signals:
    void stopTracertThread();

    void stopPingThread();

    void stopMulticastThread();

    void stopSnifferThread();

private slots:


    void exp_mySnifferFinish();

    void exp_recvSnifferData(QStringList data);

    void exp_mySnifferError(QString data);

    void on_pushButton_exp_start_clicked();

    void on_pushButton_exp_saveToFile_clicked();



private:
    Ui::MainWindow *ui;


    MySniffer* mySniffer;
    QList<QStringList> dataList;
    void init();


};

#endif // MAINWINDOW_H
