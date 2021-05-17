#ifndef GROUPSETTINGS_H
#define GROUPSETTINGS_H

#include <QMainWindow>

namespace Ui {
class groupSettings;
}

class groupSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit groupSettings(QWidget *parent = nullptr);
    ~groupSettings();

private slots:
    void  updateGroupsList();

    void updateUserGroupList();

    void removeUsersFromGroup();

    void on_groupList_itemSelectionChanged();

    void on_changeNameBtn_clicked();

    void on_deleteGroup_clicked();

    void on_promoteBtn_clicked();

    void on_demoteBtn_clicked();

    void on_addUserBtn_clicked();

    void on_removeUserBtn_clicked();

private:
    Ui::groupSettings *ui;
};

#endif // GROUPSETTINGS_H
