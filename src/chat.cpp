#include "headers/chat.h"
#include "headers/otherFunctions.h"

Chat::Chat()
{

}

int Chat::createChat()
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Insert into chats(chatID, text) VALUES (null,null)";

        string queryStr = insertQuery.str();

        //Convert String to Const Char
        const char* q = queryStr.c_str();
        int exeQuery = mysql_query(conn, q);

        if(exeQuery == 0)
        {
            stringstream selectQuery;
            selectQuery << "SELECT * FROM chats ORDER BY chatID DESC LIMIT 1";

            string queryStr = selectQuery.str();

            //Convert String to Const Char
            const char* c = queryStr.c_str();

            int getIdQuery = mysql_query(conn, c);

            if(getIdQuery == 0)
            {
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);

                if (row != nullptr)
                {
                    //Return Chat ID
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
    else
    {
        return 0;
    }

    mysql_close(conn);
}


int Chat::deleteChat(int chatID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Delete from chats where chatID ='"<< chatID << "'";

        string queryStr = insertQuery.str();

        //Convert String to Const Char
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

    mysql_close(conn);
}

string Chat::getMessages(int chatID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream selectQuery;
        selectQuery << "Select * from chats where chatID ='" << chatID << "'";

        string queryStr = selectQuery.str();

        //Convert String to Const Char
        const char* q = queryStr.c_str();

        int exeQuery = mysql_query(conn, q);

        if(!exeQuery)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            if (row != nullptr)
            {
                if(row[1] != nullptr)
                    {
                        char *chr = row[1];
                        string text(chr);
                        return text;
                    }
                    else
                    {
                        return "";
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

    mysql_close(conn);
}

