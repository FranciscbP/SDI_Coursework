#ifndef FRIENDSETTINGS_H
#define FRIENDSETTINGS_H

#include <QMainWindow>

namespace Ui {
class friendSettings;
}

class friendSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit friendSettings(QWidget *parent = nullptr);
    ~friendSettings();

private slots:
    void updateFriendsList();

    void updateFriendRequestList();

    void on_addFriendBtn_clicked();

    void on_removeFriendBtn_clicked();

    void on_acceptBtn_clicked();

    void on_declineBtn_clicked();

private:
    Ui::friendSettings *ui;
};

#endif // FRIENDSETTINGS_H
