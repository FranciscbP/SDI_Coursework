#include "headers/loginscreen.h"
#include "ui_loginscreen.h"
#include "QMessageBox"
#include "headers/header.h"
#include "headers/user.h"

loginScreen::loginScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginScreen)
{
    ui->setupUi(this);
}

loginScreen::~loginScreen()
{
    delete ui;
}


void loginScreen::on_LoginButton_clicked()
{
    User user;

    QString qEmail = ui->input_email->text();
    QString qPassword = ui->input_password->text();

    string email = qEmail.toStdString();
    string password = qPassword.toStdString();

    int resultado = user.login(email,password);

    if(resultado != 0)
    {
        QMessageBox::information(this,"User","Logged In!!");
    }
    else
    {
        QMessageBox::information(this,"User","Not Logged In!!");
    }
}
