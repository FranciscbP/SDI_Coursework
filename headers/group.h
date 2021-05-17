#ifndef GROUP_H
#define GROUP_H
#include "header.h"
#include "headers/connectdb.h"
#include "headers/otherFunctions.h"

class Group
{
public:
    Group();

    int createGroup(string);
    int addAdmin(int,int);
    int addGroupChat(int,int);

    int getID(string);
    string getGroupName(int);

    int getNumberOfUsers(int);
    int getUsers(int,int);

    int changeGroupName(int,string);
    int deleteGroup(int);

    int addUser(int,int);
    int removeUser(int,int);

    int promoteUser(int,int);
    int demoteUser(int,int);  

    int getChatID(int);
};

#endif // GROUP_H
