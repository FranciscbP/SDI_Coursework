#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void updateGroups();

    void updateGroupUsers();

    void updateFriends();

    void updateChatBox();

private slots:

    void on_send_message_btn_clicked();

    void on_DirectMessagesBtn_clicked();

    void on_friendsSettingsBtn_clicked();

    void on_friendsList_itemSelectionChanged();

    void on_group1_button_clicked();

    void on_group2_button_clicked();

    void on_group3_button_clicked();

    void on_group4_button_clicked();

    void on_group5_button_clicked();

    void on_group6_button_clicked();

    void on_group7_button_clicked();

    void openFriendsScreen();

    void openGroupScreen();

    void on_groupSettingsBtn_clicked();

    void on_settingsBtn_clicked();

    void on_add_group_button_clicked();


    void on_tabName_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
