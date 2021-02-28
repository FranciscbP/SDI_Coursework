#include "headers/user.h"
#include "headers/connectdb.h"
#include "headers/otherFunctions.h"

User::User()
{

}

int User::login(string email,string passwd)
{
    connectDB DB;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, NULL, 0);

    if(conn)
    {
        stringstream selectQuery;
        selectQuery << "Select * from users where email ='" << email << "'";

        string queryStr = selectQuery.str();
        const char* q = queryStr.c_str();

        int exeQuery = mysql_query(conn, q);

        if(!exeQuery)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            if (row != nullptr)
            {
                if(row[2] == passwd)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


int User::registr(string email, string passwd)
{
    int userExists = getID(email);

    if(userExists == 0)
    {
        connectDB DB;

        MYSQL* conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, NULL, 0);

        if(conn)
        {
            stringstream insertQuery;
            insertQuery << "INSERT INTO users(userID, email, password) VALUES ('NULL','" << email << "','" << passwd <<"')";

            string queryStr = insertQuery.str();
            const char* q = queryStr.c_str();
            int exeQuery = mysql_query(conn, q);

            if(exeQuery == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        //User Already exists;
        return 2;
    }


}

int User::getID(string email)
{
   connectDB DB;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, NULL, 0);

    if(conn)
    {
        stringstream selectQuery;
        selectQuery << "Select * from users where email ='" << email << "'";

        string queryStr = selectQuery.str();
        const char* q = queryStr.c_str();

        int exeQuery = mysql_query(conn, q);

        if(!exeQuery)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            if (row != nullptr)
            {
                char *chr = row[0];
                int id = convertCharToInt(chr);
                return id;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

