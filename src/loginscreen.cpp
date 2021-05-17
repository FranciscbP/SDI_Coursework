#include "headers/loginscreen.h"
#include "ui_loginscreen.h"
#include "headers/header.h"
#include "headers/user.h"
#include "headers/mainwindow.h"

loginScreen::loginScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginScreen)
{
    this->setFixedSize(QSize(800, 600));
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    //Hide Register Form
    ui->RegisterButton->setHidden(true);
    ui->input_username_register->setHidden(true);
    ui->input_password_register->setHidden(true);
    ui->go_loginPage->setHidden(true);
}

loginScreen::~loginScreen()
{
    delete ui;
}

//Open MainScreen Form
void loginScreen::openMainScreen()
{
    static MainWindow *mainScrn = new MainWindow(this);
    mainScrn->show();
    mainScrn->activateWindow();
    mainScrn->raise();
}

//Hide Register Form
//Show Login Form
void loginScreen::on_go_loginPage_clicked()
{
    ui->RegisterButton->setHidden(true);
    ui->input_username_register->setHidden(true);
    ui->input_password_register->setHidden(true);
    ui->go_loginPage->setHidden(true);
    ui->LoginButton->setHidden(false);
    ui->input_username->setHidden(false);
    ui->input_password->setHidden(false);
    ui->go_registerPage->setHidden(false);
}

//Hide Login Form
//Show Show Form
void loginScreen::on_go_registerPage_clicked()
{
    ui->RegisterButton->setHidden(false);
    ui->input_username_register->setHidden(false);
    ui->input_password_register->setHidden(false);
    ui->go_loginPage->setHidden(false);
    ui->LoginButton->setHidden(true);
    ui->input_username->setHidden(true);
    ui->input_password->setHidden(true);
    ui->go_registerPage->setHidden(true);
}

void loginScreen::on_LoginButton_clicked()
{
    User user;

    QString qUsername = ui->input_username->text();
    QString qPassword = ui->input_password->text();

    //Clear Inputs
    ui->input_username->clear();
    ui->input_password->clear();

    string username = qUsername.toStdString();
    string password = qPassword.toStdString();

    int userID = user.login(username,password);


    if(userID != 0)
    {
        //Successful Login
        QMessageBox::information(this,"User","Logged In!!");
        this->hide();

        openMainScreen();
    }
    else
    {
        //Unsuccessful Login
        QMessageBox::information(this,"User","Not Logged In!!");
    }
}

void loginScreen::on_RegisterButton_clicked()
{
    User user;

    QString qUsername = ui->input_username_register->text();
    QString qPassword = ui->input_password_register->text();

    //Clear Inputs
    ui->input_username_register->clear();
    ui->input_password_register->clear();

    string username = qUsername.toStdString();
    string password = qPassword.toStdString();

    int tryRegister = user.registr(username,password);

    if(tryRegister == 1)
    {
        //Successful Register
        QMessageBox::information(this,"User","Registered!!");
        ui->RegisterButton->setHidden(true);
        ui->input_username_register->setHidden(true);
        ui->input_password_register->setHidden(true);
        ui->go_loginPage->setHidden(true);
        ui->LoginButton->setHidden(false);
        ui->input_username->setHidden(false);
        ui->input_password->setHidden(false);
        ui->go_registerPage->setHidden(false);
    }
    else if(tryRegister == 2)
    {
        //Unsuccessful Register - User Already on Database
        QMessageBox::information(this,"User","Already Exists!!");
    }
    else
    {
       QMessageBox::information(this,"Database","User Not Created!!");
    }
}



