#include "headers/friendsettings.h"
#include "ui_friendsettings.h"
#include "headers/user.h"
#include "qinputdialog.h"
#include "headers/mainwindow.h"
#include "headers/chat.h"

int uID;

friendSettings::friendSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::friendSettings)
{
    this->setFixedSize(QSize(600, 550));
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->friendList->setStyleSheet("QListView::item:selected{background-color: rgb(255,0,0);}QListWidget{background-color: rgb(85, 87, 83);border-radius:0px;}");
    ui->friendRequestList->setStyleSheet("QListView::item:selected{background-color: rgb(255,0,0);}QListWidget{background-color: rgb(85, 87, 83);border-radius:0px;}");

    //Get User ID
    User user;
    uID = user.getLoggedUser();

    updateFriendsList();
    updateFriendRequestList();

    ui->friendList->setCurrentRow(0);
    ui->friendRequestList->setCurrentRow(0);
}

friendSettings::~friendSettings()
{
    delete ui;
    ui->friendList->setCurrentRow(0);
    ui->friendRequestList->setCurrentRow(0);
}

void friendSettings::updateFriendsList()
{
    ui->friendList->clear();

    User user;
    int numberOfFriends = user.getNumberOfFriends(uID);

    for (int i = 0; i< numberOfFriends; i++)
    {
        int friendID = user.getFriends(uID, i);

        string friendName = user.getUsername(friendID);
        QString qFriendName = QString::fromStdString(friendName);

        ui->friendList->addItem(qFriendName);
    }

}

void friendSettings::updateFriendRequestList()
{
    ui->friendRequestList->clear();

    User user;
    int numberOfFriends = user.getNumberOfFriendRequests(uID);

    for (int i = 0; i< numberOfFriends; i++)
    {
        int friendID = user.getFriendRequests(uID, i);

        string friendName = user.getUsername(friendID);
        QString qFriendName = QString::fromStdString(friendName);

        ui->friendRequestList->addItem(qFriendName);
    }
}


void friendSettings::on_addFriendBtn_clicked()
{
    User user;

    QString qText = QInputDialog::getText(this, "Add Friend","Username: ", QLineEdit::Normal);
    string text = qText.QString::toStdString();

    int getFriendID = user.getID(text);

    if(getFriendID != uID)
    {
        if(getFriendID != 0)
        {
            int addFriend = user.sendFriendRequest(uID,getFriendID);

            if(addFriend == 1)
            {
                QMessageBox::information(this,"Friend","Friend Request Sent!");
            }
            else if(addFriend == 2)
            {
                QMessageBox::information(this,"Friend","User is Already your Friend!");
            }
            else if(addFriend ==3)
            {
                QMessageBox::information(this,"Friend","Friend Request Already Sent!");
            }
            else
             {
                 QMessageBox::information(this,"Error","Error!");
             }
        }
        else
        {
            QMessageBox::information(this,"Friend","Username does not Exist!");
        }

    }
    else
    {
        QMessageBox::information(this,"Friend","You can not Add Yourself!");
    }

}

void friendSettings::on_removeFriendBtn_clicked()
{
    User user;

   int countFriends = ui->friendList->count();

   if(countFriends != 0)
   {
       QString itemName = ui->friendList->currentItem()->text();

       QMessageBox::StandardButton reply;
       reply = QMessageBox::question(this, "Friend", "Do you want to Remove " +itemName + " from your Friends?",QMessageBox::Yes|QMessageBox::No);

       if (reply == QMessageBox::Yes)
       {
         string friendName = itemName.QString::toStdString();
         int friendID = user.getID(friendName);

         int remove = user.removeFriend(uID,friendID);

         if(remove != 0)
         {
             QMessageBox::information(this,"Friend",itemName+" was Successfully Removed from your Friends!");
             updateFriendsList();
             ui->friendList->setCurrentRow(0);

         }
         else
         {
             QMessageBox::information(this,"Friend","Error Removing Friend!");
         }
       }
   }
   else
   {
        QMessageBox::information(this,"Friend","No Friends to Remove!");
   }

}

void friendSettings::on_acceptBtn_clicked()
{
    User user;
    Chat chat;

    int countRequests = ui->friendRequestList->count();

    if(countRequests != 0)
    {
        QString itemName = ui->friendRequestList->currentItem()->text();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Friend", "Do you want to Accept " +itemName + " as your Friend?",QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
          string friendName = itemName.QString::toStdString();
          int friendID = user.getID(friendName);

          int accept = user.acceptFriendRequest(uID,friendID);

          if(accept != 0)
          {
              QMessageBox::information(this,"Friend",itemName+" was Successfully Added to your Friends!");
              updateFriendsList();
              updateFriendRequestList();
              ui->friendList->setCurrentRow(0);
              ui->friendRequestList->setCurrentRow(0);

              //Chat
              int chatID = chat.createChat();
              user.addFriendChat(uID,friendID,chatID);

          }
          else
          {
              QMessageBox::information(this,"Friend","Error Adding Friend!");
          }
        }
    }
    else
    {
        QMessageBox::information(this,"Friend","No Requests to Accept!");
    }


}

void friendSettings::on_declineBtn_clicked()
{
    User user;

    int countRequests = ui->friendRequestList->count();

    if(countRequests != 0)
    {
        QString itemName = ui->friendRequestList->currentItem()->text();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Friend", "Do you want to Decline " +itemName + " as your Friend?",QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
          string friendName = itemName.QString::toStdString();
          int friendID = user.getID(friendName);

          int accept = user.declineFriendRequest(uID,friendID);

          if(accept != 0)
          {
              QMessageBox::information(this,"Friend","Friend Request Declined!");
              updateFriendsList();
              updateFriendRequestList();
              ui->friendList->setCurrentRow(0);
              ui->friendRequestList->setCurrentRow(0);
          }
          else
          {
              QMessageBox::information(this,"Friend","Error Declining Friend!");
          }
        }
    }
    else
    {
        QMessageBox::information(this,"Friend","No Requests to Decline!");
    }
}
