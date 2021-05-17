#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/header.h"
#include "headers/publisher.h"
#include "headers/group.h"
#include "headers/user.h"
#include "headers/friendsettings.h"
#include "headers/groupsettings.h"
#include "headers/chat.h"
#include "qinputdialog.h"

int userID;
vector <int> userGroups;
int gID;
int currentChat;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setFixedSize(QSize(999, 600));
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    //Get User ID after login
    User user;
    userID = user.getLoggedUser();

    string userName = user.getUsername(userID);
    QString qUserName = QString::fromStdString(userName);

    ui->settingsBtn->setText(qUserName);
    ui->friendsList->setStyleSheet("QListView::item:selected{background-color: rgb(255,0,0);}");

    ui->group1_button->setVisible(false);
    ui->group2_button->setVisible(false);
    ui->group3_button->setVisible(false);
    ui->group4_button->setVisible(false);
    ui->group5_button->setVisible(false);
    ui->group6_button->setVisible(false);
    ui->group7_button->setVisible(false);
    ui->add_group_button->setVisible(false);

    updateGroups();
    updateFriends();
    ui->groupUsersList->setVisible(false);
    ui->groupUsersList->setEnabled(false);
    ui->groupSettingsBtn->setVisible(false);

    ui->friendsList->setCurrentRow(0);

    int i = ui->friendsList->count();
    if(i!= 0)
    {
        QString itemName = ui->friendsList->currentItem()->text();
        string friendName = itemName.QString::toStdString();

        int friendID = user.getID(friendName);
        currentChat = user.getChatID(userID,friendID);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_send_message_btn_clicked()
{
    //Get User Name
    User user;
    string userName = user.getUsername(userID);

    QString qMessageText = ui->message_text->toPlainText();

    string currentChatStr = to_string(currentChat);

    string strMessageText = currentChatStr + "-" + userName + ": " + qMessageText.toStdString();

    //Convert String to Const Char
    const char *messageText = strMessageText.c_str();

    //Convert Int to String
    string clientID = to_string(userID);

    //Send Message
    publisher(messageText,"ChatRoom",clientID);

    ui->message_text->clear();
}

void MainWindow::on_DirectMessagesBtn_clicked()
{
   User user;

   updateFriends();
   ui->friendsList->setCurrentRow(0);

   int i = ui->friendsList->count();
   if(i!= 0)
   {
       QString itemName = ui->friendsList->currentItem()->text();
       string friendName = itemName.QString::toStdString();

       int friendID = user.getID(friendName);
       currentChat = user.getChatID(userID,friendID);
   }


   ui->friendsList->setVisible(true);
   ui->groupUsersList->setVisible(false);

   ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
   ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
   ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
   ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
   ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
   ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
   ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
   ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

   ui->friendsSettingsBtn->setVisible(true);
   ui->groupSettingsBtn->setVisible(false);
}

void MainWindow::on_friendsSettingsBtn_clicked()
{
    openFriendsScreen();
}

void MainWindow::updateFriends()
{
    ui->friendsList->clear();

    User user;
    int numberOfFriends = user.getNumberOfFriends(userID);

    for (int i = 0; i< numberOfFriends; i++)
    {
        int friendID = user.getFriends(userID, i);

        string friendName = user.getUsername(friendID);
        QString qFriendName = QString::fromStdString(friendName);

        ui->friendsList->addItem(qFriendName);
    }
}

void MainWindow::on_friendsList_itemSelectionChanged()
{
    User user;
    QString itemName = ui->friendsList->currentItem()->text();

    ui->tabName->setText(itemName);

    string friendName = itemName.QString::toStdString();

    int friendID = user.getID(friendName);
    currentChat = user.getChatID(userID,friendID);
}

void MainWindow::updateGroups()
{
    User user;
    Group group;

    int numberOfGroups = user.getNumberOfGroups(userID);

    ui->group1_button->setVisible(false);
    ui->group2_button->setVisible(false);
    ui->group3_button->setVisible(false);
    ui->group4_button->setVisible(false);
    ui->group5_button->setVisible(false);
    ui->group6_button->setVisible(false);
    ui->group7_button->setVisible(false);

    if(numberOfGroups != 0)
    {
        vector <int> groups;

        for (int i = 0; i< numberOfGroups; i++)
        {
            int groupID = user.getGroups(userID, i);
            groups.push_back(groupID);
        }

        userGroups = groups;

        for(int i = 0; i < numberOfGroups; i++)
        {
            switch(i + 1)
            {
                case 1:
                        {
                            ui->group1_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group1_button->setText(qGroupName);

                            ui->add_group_button->setVisible(true);
                            ui->add_group_button->setGeometry(85,70,41,41);
                            break;
                        }
                case 2:
                        {
                            ui->group2_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group2_button->setText(qGroupName);

                            ui->add_group_button->setVisible(true);
                            ui->add_group_button->setGeometry(85,130,41,41);
                            break;
                        }
                case 3:
                        {
                            ui->group3_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group3_button->setText(qGroupName);

                            ui->add_group_button->setVisible(true);
                            ui->add_group_button->setGeometry(85,190,41,41);
                            break;
                        }
                case 4:
                        {
                            ui->group4_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group4_button->setText(qGroupName);

                            ui->add_group_button->setVisible(true);
                            ui->add_group_button->setGeometry(85,250,41,41);
                            break;
                        }
                case 5:
                        {
                            ui->group5_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group5_button->setText(qGroupName);

                            ui->add_group_button->setVisible(true);
                            ui->add_group_button->setGeometry(85,310,41,41);
                            break;
                        }
                case 6:
                        {
                            ui->group6_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group6_button->setText(qGroupName);

                            ui->add_group_button->setVisible(true);
                            ui->add_group_button->setGeometry(85,370,41,41);
                            break;
                        }
                case 7:
                        {
                            ui->group7_button->setVisible(true);

                            string groupName = group.getGroupName(userGroups[i]);
                            QString qGroupName = QString::fromStdString(groupName);

                            ui->group7_button->setText(qGroupName);

                            ui->add_group_button->setVisible(false);
                            break;
                        }
            }

        }
    }
    else
    {
        ui->add_group_button->setVisible(true);
        ui->add_group_button->setGeometry(80,20,41,41);
    }
}

void MainWindow::updateGroupUsers()
{
    ui->groupUsersList->clear();

    Group group;
    User user;

    int numberOfGroupUsers = group.getNumberOfUsers(gID);

    for(int i = 0; i < numberOfGroupUsers; i++)
    {
        int groupUserId = group.getUsers(gID,i);

        string groupUserName = user.getUsername(groupUserId);
        QString qGroupUserName = QString::fromStdString(groupUserName);

        ui->groupUsersList->addItem(qGroupUserName);
    }
}

void MainWindow::on_group1_button_clicked()
{
    Group group;

    QString buttonText = ui->group1_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::on_group2_button_clicked()
{
    Group group;

    QString buttonText = ui->group2_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::on_group3_button_clicked()
{
    Group group;

    QString buttonText = ui->group3_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::on_group4_button_clicked()
{
    Group group;

    QString buttonText = ui->group4_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::on_group5_button_clicked()
{
    Group group;

    QString buttonText = ui->group5_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::on_group6_button_clicked()
{
    Group group;

    QString buttonText = ui->group6_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::on_group7_button_clicked()
{
    Group group;

    QString buttonText = ui->group7_button->text();
    string groupName = buttonText.toStdString();

    gID = group.getID(groupName);

    updateGroupUsers();

    currentChat = group.getChatID(gID);

    ui->tabName->setText(buttonText);

    ui->groupUsersList->setVisible(true);
    ui->friendsList->setVisible(false);

    ui->DirectMessagesBtn->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group1_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group2_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group3_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group4_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group5_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group6_button->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);border-radius:16px;}QPushButton:hover{background-color:white;}");
    ui->group7_button->setStyleSheet("QPushButton{background-color: white;border-radius:16px;} QPushButton:hover{background-color:white;}");

    ui->friendsSettingsBtn->setVisible(false);
    ui->groupSettingsBtn->setVisible(true);
}

void MainWindow::openFriendsScreen()
{
    User user;
    user.setLoggedUser(userID);

    static friendSettings *friendScreen = new friendSettings(this);
    friendScreen->show();
    friendScreen->activateWindow();
    friendScreen->raise();
}


void MainWindow::openGroupScreen()
{
    User user;
    user.setLoggedUser(userID);

    static groupSettings *groupScreen = new groupSettings(this);
    groupScreen->show();
    groupScreen->activateWindow();
    groupScreen->raise();
}

void MainWindow::on_groupSettingsBtn_clicked()
{
    openGroupScreen();
}

void MainWindow::on_settingsBtn_clicked()
{

}

void MainWindow::on_add_group_button_clicked()
{
    User user;
    Group group;
    Chat chat;

    QString qText = QInputDialog::getText(this, "Group","Group Name: ", QLineEdit::Normal);
    string text = qText.QString::toStdString();

    int nameExists = group.getID(text);

    if(nameExists == 0)
    {
        int createGroup = group.createGroup(text);

        if(createGroup != 0)
        {
            int addAdmin = group.addAdmin(createGroup,userID);

            if(addAdmin != 0)
            {
                QMessageBox::information(this,"Group","Group Successfully Created!");
                int chatID = chat.createChat();
                group.addGroupChat(createGroup,chatID);

                updateGroups();
            }
            else
            {
                QMessageBox::information(this,"Group","Error Adding Group");
            }
        }
        else
        {
            QMessageBox::information(this,"Group","Group Not Created!");
        }

    }
    else
    {
        QMessageBox::information(this,"Group","Group Name Unavailable!");
    }
}

void MainWindow::updateChatBox()
{
    Chat chat;
    string chatText = chat.getMessages(currentChat);
    QString qChatText = QString::fromStdString(chatText);

    ui->chat->setText(qChatText);
}

void MainWindow::on_tabName_clicked()
{
    updateChatBox();
}
