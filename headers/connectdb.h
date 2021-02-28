#ifndef CONNECTDB_H
#define CONNECTDB_H
#include "header.h"

class connectDB
{
public:
    connectDB();

    const char *server;
    const char *user;
    const char *passwd;
    const char *db;
};

#endif // CONNECTDB_H
