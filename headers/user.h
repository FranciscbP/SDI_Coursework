#ifndef USER_H
#define USER_H
#include "header.h"

class User
{
public:
    User();

    int login(string,string);
    int registr(string,string);

    int getID(string);


/*
    string getName(int);
    void changeIsOnline();
    void getNotifications();
*/
};

#endif // USER_H
