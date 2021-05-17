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
    string getUsername(int);

    void setLoggedUser(int);
    int  getLoggedUser();

    string createHash(string);
    string convertHashToString(size_t);

    int checkFriendShip(int,int);
    int sendFriendRequest(int, int);

    int removeFriend(int,int);
    int acceptFriendRequest(int,int);
    int declineFriendRequest(int,int);

    int addFriendChat(int,int,int);

    int getNumberOfFriends(int);
    int getFriends(int,int);

    int getNumberOfFriendRequests(int);
    int getFriendRequests(int,int);

    int getNumberOfGroups(int);
    int getGroups(int,int);

    int checkGroupPermissions(int,int);
    int checkUserInGroup(int,int);

    int getChatID(int,int);

};

#endif // USER_H
