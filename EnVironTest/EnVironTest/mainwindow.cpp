#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_path->setText("..\\");
    ui->lineEdit_FileName->setText("test.bmp");
    ui->label->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString PathAndName=ui->lineEdit_path->text()+ui->lineEdit_FileName->text();
    QPixmap img;
    img.load(PathAndName,nullptr,Qt::AutoColor);
    ui->label->setPixmap(img);
}
