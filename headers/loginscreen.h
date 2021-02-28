#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class loginScreen; }
QT_END_NAMESPACE

class loginScreen : public QMainWindow
{
    Q_OBJECT

public:
    loginScreen(QWidget *parent = nullptr);
    ~loginScreen();

private slots:
    void on_LoginButton_clicked();

private:
    Ui::loginScreen *ui;
};
#endif // LOGINSCREEN_H
