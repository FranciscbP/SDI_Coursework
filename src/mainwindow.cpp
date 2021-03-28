#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/header.h"
#include "headers/publisher.hpp"
#include "headers/user.h"

int userID;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    User user;
    userID = user.getLoggedUser();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_send_message_btn_clicked()
{
    User user;
    string userEmail = user.getEmail(userID);

    QString qMessageText = ui->message_text->toPlainText();
    string strMessageText = userEmail + ": " + qMessageText.toStdString();
    const char *messageText = strMessageText.c_str();

    string clientID = to_string(userID);

    publisher(messageText,"chatroom",clientID);

    ui->message_text->clear();
}
