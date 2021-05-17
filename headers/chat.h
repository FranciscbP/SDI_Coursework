#ifndef CHAT_H
#define CHAT_H
#include "header.h"
#include "headers/connectdb.h"

class Chat
{
public:
    Chat();

    int createChat();
    int deleteChat(int);

    string getMessages(int);
};

#endif // CHAT_H
