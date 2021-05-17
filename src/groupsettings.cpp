#include "headers/groupsettings.h"
#include "ui_groupsettings.h"
#include "headers/user.h"
#include "headers/group.h"
#include "qinputdialog.h"
#include "headers/mainwindow.h"


int usrID;
int grID;

groupSettings::groupSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::groupSettings)
{
    this->setFixedSize(QSize(600, 600));
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->groupList->setStyleSheet("QListView::item:selected{background-color: rgb(255,0,0);}QListWidget{background-color: rgb(85, 87, 83);border-radius:0px;}");
    ui->groupUsersList->setStyleSheet("QListView::item:selected{background-color: rgb(255,0,0);}QListWidget{background-color: rgb(85, 87, 83);border-radius:0px;}");

    //getUser ID
    User user;
    usrID = user.getLoggedUser();

    updateGroupsList();

}

groupSettings::~groupSettings()
{
    delete ui;
}

void groupSettings::updateGroupsList()
{
    ui->groupList->clear();

    User user;
    Group group;

    int numberOfGroups = user.getNumberOfGroups(usrID);

    if(numberOfGroups != 0)
    {
        for(int i = 0; i < numberOfGroups; i++)
        {
            int groupID = user.getGroups(usrID,i);

            string groupName = group.getGroupName(groupID);
            QString qGroupName = QString::fromStdString(groupName);

            ui->groupList->addItem(qGroupName);

            if(i == 0)
            {
                grID = groupID;
            }
        }

        ui->groupList->setCurrentRow(0);
        updateUserGroupList();
    }
}

void groupSettings::updateUserGroupList()
{

    ui->groupUsersList->clear();

    Group group;
    User user;

    int numberOfGroupUsers = group.getNumberOfUsers(grID);

    for(int i = 0; i < numberOfGroupUsers; i++)
    {
        int groupUserId = group.getUsers(grID,i);
        int userRole = user.checkGroupPermissions(groupUserId,grID);

        string groupUserName = user.getUsername(groupUserId);
        QString qGroupUserName = QString::fromStdString(groupUserName);

        if(userRole == 0)
        {
            ui->groupUsersList->addItem(qGroupUserName + " - User");
        }
        else if(userRole == 1)
        {
            ui->groupUsersList->addItem(qGroupUserName + " - Moderator");
        }
        else if(userRole == 2)
        {
            ui->groupUsersList->addItem(qGroupUserName + " - Admin");
        }

    }

    ui->groupUsersList->setCurrentRow(0);

}

void groupSettings::on_groupList_itemSelectionChanged()
{
    Group group;

    QString itemName = ui->groupList->currentItem()->text();
    string gName = itemName.QString::toStdString();

    grID = group.getID(gName);

    updateUserGroupList();
    ui->groupUsersList->setCurrentRow(0);

}

void groupSettings::on_changeNameBtn_clicked()
{
    User user;
    Group group;

   int countGroups = ui->groupList->count();

   if(countGroups != 0)
   {
      int permission = user.checkGroupPermissions(usrID,grID);

      if(permission != 0)
      {
          QString qText = QInputDialog::getText(this, "Group","Change Group Name: ", QLineEdit::Normal);
          string text = qText.QString::toStdString();

          int nameExists = group.getID(text);

          if(nameExists == 0)
          {
              int change = group.changeGroupName(grID,text);
              if(change != 0)
              {
                QMessageBox::information(this,"Group","Name Changed!");
              }
              else
              {
                  QMessageBox::information(this,"Group","Error!");
              }

          }
          else
          {
              QMessageBox::information(this,"Group","Group Name Unavailable!");
          }
      }
      else
      {
          QMessageBox::information(this,"Group","You don't have Permissions to change Group Name!");
      }

   }
   else
   {
        QMessageBox::information(this,"Group","No Groups to Change Name!");
   }

   updateGroupsList();

}

void groupSettings::on_deleteGroup_clicked()
{
    User user;
    Group group;

   int countGroups = ui->groupUsersList->count();

   if(countGroups != 0)
   {
      int permission = user.checkGroupPermissions(usrID,grID);

      if(permission != 0)
      {
        removeUsersFromGroup();

        int deleted = group.deleteGroup(grID);

        if(deleted != 0)
        {
            QMessageBox::information(this,"Group","Successfully Deleted!");
        }
        else
        {
            QMessageBox::information(this,"Group","Error!");
        }
      }
      else
      {
          QMessageBox::information(this,"Group","You don't have Permissions to Delete the Group!");
      }

   }
   else
   {
        QMessageBox::information(this,"Group","No Groups to Delete!");
   }

   updateGroupsList();
}

void groupSettings::removeUsersFromGroup()
{
    User user;
    Group group;

    int numberOfGroupUsers = group.getNumberOfUsers(grID);

    for(int i = 0; i < numberOfGroupUsers; i++)
    {
        int groupUserId = group.getUsers(grID,i);

        group.removeUser(grID,groupUserId);
    }
}

void groupSettings::on_promoteBtn_clicked()
{
    Group group;
    User user;

    int permission = user.checkGroupPermissions(usrID,grID);

    if(permission != 0)
    {
        int countUsers = ui->groupUsersList->count();

        if(countUsers != 0)
        {
            QString itemName = ui->groupUsersList->currentItem()->text();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Group", "Do you want to Promote " +itemName + "?",QMessageBox::Yes|QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
              string userNameWithRole = itemName.QString::toStdString();
              string userName = userNameWithRole.substr(0, userNameWithRole.find(" - "));

              int userID = user.getID(userName);

              int userPermission = user.checkGroupPermissions(userID,grID);

              if(userPermission == 0)
              {
                  int promote = group.promoteUser(grID,userID);

                  if(promote != 0)
                  {
                      QMessageBox::information(this,"Group","User Promoted!");
                      updateUserGroupList();
                  }
                  else
                  {
                      QMessageBox::information(this,"Group","Error Promoting User!");
                  }
              }
              else if(userPermission == 2)
              {
                  QMessageBox::information(this,"Group","User Selected is the Group Admin!");
              }
              else
              {
                  QMessageBox::information(this,"Group","User Already Set as Moderator!");
              }

            }
        }
        else
        {
            QMessageBox::information(this,"Group","No Users to Promote!");
        }
    }
    else
    {
        QMessageBox::information(this,"Group","You don't have Permissions!");
    }


}

void groupSettings::on_demoteBtn_clicked()
{
    Group group;
    User user;

    int permission = user.checkGroupPermissions(usrID,grID);

    if(permission != 0)
    {
        int countUsers = ui->groupUsersList->count();

        if(countUsers != 0)
        {
            QString itemName = ui->groupUsersList->currentItem()->text();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Group", "Do you want to Demote " +itemName + "?",QMessageBox::Yes|QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
              string userNameWithRole = itemName.QString::toStdString();
              string userName = userNameWithRole.substr(0, userNameWithRole.find(" - "));
              int userID = user.getID(userName);

              int userPermission = user.checkGroupPermissions(userID,grID);

                  if(userPermission == 1)
                  {
                      int demote = group.demoteUser(grID,userID);

                      if(demote != 0)
                      {
                          QMessageBox::information(this,"Group","User Demoted!");
                          updateUserGroupList();
                      }
                      else
                      {
                          QMessageBox::information(this,"Group","Error Demoting User!");
                      }
                  }
                  else if(userPermission == 2)
                  {
                      QMessageBox::information(this,"Group","User Selected is the Group Admin!");
                  }
                  else
                  {
                      QMessageBox::information(this,"Group","User Already with no Permissions!");
                  }
            }
        }
        else
        {
            QMessageBox::information(this,"Group","No Users to Demote!");
        }
    }
    else
    {
        QMessageBox::information(this,"Group","You don't have Permissions!");
    }
}

void groupSettings::on_addUserBtn_clicked()
{
    User user;
    Group group;

    int countGroups = ui->groupList->count();

    if(countGroups != 0)
    {
       int permission = user.checkGroupPermissions(usrID,grID);

       if(permission != 0)
       {
           QString qText = QInputDialog::getText(this, "Group","Add User to Group: ", QLineEdit::Normal);
           string text = qText.QString::toStdString();
           int userID = user.getID(text);

           if (userID != 0)
           {
               int checkUser = user.checkUserInGroup(userID,grID);
               if(checkUser == 0)
               {
                   int checkNumberOfGroups = user.getNumberOfGroups(userID);

                   cout << checkNumberOfGroups;

                   if(checkNumberOfGroups < 7)
                   {
                       int addUser = group.addUser(grID,userID);
                       if(addUser != 0)
                       {
                            QMessageBox::information(this,"Group","User Added to Group!");
                            updateUserGroupList();
                       }
                       else
                       {
                            QMessageBox::information(this,"Group","Could not Add User to Group!");
                       }
                   }
                   else
                   {
                       QMessageBox::information(this,"Group","User Limit of Groups Exceeded!");
                   }
               }
               else
               {
                   QMessageBox::information(this,"Group","User Already on Group!");
               }
           }
           else
           {
               QMessageBox::information(this,"Group","User does not Exist!");
           }

       }
       else
       {
           QMessageBox::information(this,"Group","You don't have Permissions to change Group Name!");
       }

    }
    else
    {
         QMessageBox::information(this,"Group","No Groups to Change Name!");
    }
}

void groupSettings::on_removeUserBtn_clicked()
{
    Group group;
    User user;

    int permission = user.checkGroupPermissions(usrID,grID);

    if(permission != 0)
    {
        int countUsers = ui->groupUsersList->count();

        if(countUsers != 0)
        {
            QString itemName = ui->groupUsersList->currentItem()->text();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Group", "Do you want to Remove " +itemName + "?",QMessageBox::Yes|QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
              string userNameWithRole = itemName.QString::toStdString();
              string userName = userNameWithRole.substr(0, userNameWithRole.find(" - "));
              int userID = user.getID(userName);

              int userPermission = user.checkGroupPermissions(userID,grID);

              if(userPermission != 2)
              {
                  int remove = group.removeUser(grID,userID);
                  if(remove != 0)
                  {
                      QMessageBox::information(this,"Group","User was Removed from the Group!");
                      updateUserGroupList();
                  }
                  else
                  {
                      QMessageBox::information(this,"Group","Error Removing User!");
                  }
              }
              else
              {
                  QMessageBox::information(this,"Group","Admin Cannot be Removed!");
              }
            }
        }
        else
        {
            QMessageBox::information(this,"Group","No Users to Remove!");
        }
    }
    else
    {
        QMessageBox::information(this,"Group","You don't have Permissions!");
    }
}
