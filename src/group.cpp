#include "headers/group.h"
#include "headers/chat.h"

Group::Group()
{

}

int Group::createGroup(string groupName)
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
        insertQuery << "Insert into sdi_project.groups(groupID, groupName, chatID) VALUES (null,'" << groupName << "',null)";

        string queryStr = insertQuery.str();

        //Convert String to Const Char
        const char* q = queryStr.c_str();
        int exeQuery = mysql_query(conn, q);

        if(exeQuery == 0)
        {
            stringstream selectQuery;
            selectQuery << "SELECT * FROM sdi_project.groups ORDER BY groupID DESC LIMIT 1";

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
                    //Return Group ID
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

int Group::addAdmin(int groupID ,int userID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Insert into user_groups(userID, groupID, role) VALUES ('"<< userID <<"','" << groupID << "',2)";

        string queryStr = insertQuery.str();

        //Convert String to Const Char
        const char* q = queryStr.c_str();
        int exeQuery = mysql_query(conn, q);

        if(exeQuery == 0)
        {
            //User added
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

int Group::addGroupChat(int groupID, int chatID)
{

    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "UPDATE sdi_project.groups SET chatID = '"<< chatID <<"' where groupID = '"<< groupID <<"'";

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

int Group::getID(string groupName)
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
        selectQuery << "Select * from sdi_project.groups where groupName ='" << groupName << "'";

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
                //Return User ID
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

    mysql_close(conn);
}

string Group::getGroupName(int id)
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
         selectQuery << "Select * from sdi_project.groups where groupID ='" << id << "'";

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
                 //Return Username
                 char *chr = row[1];
                 string groupname(chr);
                 return groupname;
             }
             else
             {
                 return "0";
             }
         }
         else
         {
             return "0";
         }
     }
     else
     {
         return "0";
     }

     mysql_close(conn);
}

int Group::getNumberOfUsers(int groupId)
{
    //Connect to Database
    connectDB DB;

     MYSQL* conn;
     MYSQL_RES* res;
     conn = mysql_init(0);
     conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

     if(conn)
     {
         stringstream selectQuery;
         selectQuery << "Select * from sdi_project.user_groups where groupID = '" << groupId << "'";

         string queryStr = selectQuery.str();

         //Convert String to Const Char
         const char* q = queryStr.c_str();

         int exeQuery = mysql_query(conn, q);

         if(!exeQuery)
         {
             res = mysql_store_result(conn);

             int num_rows = mysql_num_rows(res);

             return num_rows;

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

int Group::getUsers(int groupId, int userRow)
{
    int userId = 0;

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
         selectQuery << "Select * from sdi_project.user_groups where groupID = '" << groupId << "'";

         string queryStr = selectQuery.str();

         //Convert String to Const Char
         const char* q = queryStr.c_str();

         int exeQuery = mysql_query(conn, q);

         if(!exeQuery)
         {
             res = mysql_store_result(conn);

             int num_rows = mysql_num_rows(res);

             if(num_rows != 0)
             {
                 int counter = 0;

                 while(row = mysql_fetch_row(res))
                 {
                    //COnvert data to int
                    char *chr0 = row[0];
                    int row0 = convertCharToInt(chr0);


                    if(counter == userRow)
                    {
                        userId = row0;
                    }

                    counter = counter + 1;
                 }

                 return userId;

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

int Group::changeGroupName(int groupID, string newName)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "UPDATE sdi_project.groups SET groupName = '"<< newName <<"' where groupID = '"<< groupID <<"'";

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

int Group::deleteGroup(int groupID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Delete FROM sdi_project.groups where groupID = '"<< groupID <<"'";

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

int Group::addUser(int groupID, int userID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Insert into sdi_project.user_groups(userID, groupID, role) VALUES ('"<< userID <<"','" << groupID << "',0)";

        string queryStr = insertQuery.str();

        //Convert String to Const Char
        const char* q = queryStr.c_str();
        int exeQuery = mysql_query(conn, q);

        if(exeQuery == 0)
        {
            //User added
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


int Group::removeUser(int groupID, int userID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Delete FROM sdi_project.user_groups where groupID = '"<< groupID <<"' and userID = '" << userID << "'";

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

int Group::promoteUser(int groupID, int userID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "UPDATE sdi_project.user_groups SET role = 1 where userID = '"<< userID <<"' AND groupID = '" << groupID <<"'";

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

int Group::demoteUser(int groupID, int userID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "UPDATE sdi_project.user_groups SET role = 0 where userID = '"<< userID <<"' AND groupID = '" << groupID <<"'";

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

int Group::getChatID(int groupID)
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
        selectQuery << "Select * from sdi_project.groups where groupID ='" << groupID << "'";

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
                //Return User ID
                char *chr = row[2];
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

    mysql_close(conn);
}
