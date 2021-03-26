#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/loginscreen.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_12_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{

}
