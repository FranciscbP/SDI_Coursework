#include "headers/loginscreen.h"
#include "ui_loginscreen.h"
#include "QMessageBox"
#include "headers/header.h"
#include "headers/user.h"
#include "headers/mainwindow.h"

loginScreen::loginScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginScreen)
{
    ui->setupUi(this);
    ui->RegisterButton->setHidden(true);
    ui->input_email_register->setHidden(true);
    ui->input_password_register->setHidden(true);
    ui->go_loginPage->setHidden(true);
}

loginScreen::~loginScreen()
{
    delete ui;
}

void loginScreen::openMainScreen()
{
  static MainWindow *mainScrn = new MainWindow(this);
  mainScrn->show();
  mainScrn->activateWindow();
  mainScrn->raise();
}

void loginScreen::on_go_loginPage_clicked()
{
    ui->RegisterButton->setHidden(true);
    ui->input_email_register->setHidden(true);
    ui->input_password_register->setHidden(true);
    ui->go_loginPage->setHidden(true);
    ui->LoginButton->setHidden(false);
    ui->input_email->setHidden(false);
    ui->input_password->setHidden(false);
    ui->go_registerPage->setHidden(false);
}

void loginScreen::on_go_registerPage_clicked()
{
    ui->RegisterButton->setHidden(false);
    ui->input_email_register->setHidden(false);
    ui->input_password_register->setHidden(false);
    ui->go_loginPage->setHidden(false);
    ui->LoginButton->setHidden(true);
    ui->input_email->setHidden(true);
    ui->input_password->setHidden(true);
    ui->go_registerPage->setHidden(true);
}


void loginScreen::on_LoginButton_clicked()
{
    User user;

    QString qEmail = ui->input_email->text();
    QString qPassword = ui->input_password->text();

    string email = qEmail.toStdString();
    string password = qPassword.toStdString();

    int tryLogin = user.login(email,password);

    if(tryLogin != 0)
    {
        QMessageBox::information(this,"User","Logged In!!");
        this->hide();

        openMainScreen();
    }
    else
    {
        QMessageBox::information(this,"User","Not Logged In!!");
    }
}

void loginScreen::on_RegisterButton_clicked()
{
    User user;

    QString qEmail = ui->input_email_register->text();
    QString qPassword = ui->input_password_register->text();

    string email = qEmail.toStdString();
    string password = qPassword.toStdString();

    int tryRegister = user.registr(email,password);

    if(tryRegister == 1)
    {
        QMessageBox::information(this,"User","Registered!!");
        ui->RegisterButton->setHidden(true);
        ui->input_email_register->setHidden(true);
        ui->input_password_register->setHidden(true);
        ui->go_loginPage->setHidden(true);
        ui->LoginButton->setHidden(false);
        ui->input_email->setHidden(false);
        ui->input_password->setHidden(false);
        ui->go_registerPage->setHidden(false);
    }
    else if(tryRegister == 2)
    {
        QMessageBox::information(this,"User","Already Exists!!");
    }
    else
    {
       QMessageBox::information(this,"Database","Problem!!");
    }
}



