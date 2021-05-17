#include "headers/user.h"
#include "headers/connectdb.h"
#include "headers/otherFunctions.h"

const string FILE_DEST = "log.txt";

User::User()
{

}

//Login
int User::login(string username,string passwd)
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
            selectQuery << "Select * from users where userName ='" << username << "'";

            string queryStr = selectQuery.str();

            //Convert String to Const Char
            const char* q = queryStr.c_str();

            int exeQuery = mysql_query(conn, q);

            if(!exeQuery)
            {
                //User Found
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);

                if (row != nullptr)
                {
                    //Encrypt Password
                    string encryptedPasswd = createHash(passwd);

                    if(row[2] == encryptedPasswd)
                    {
                        //Password is Correct
                        int userID = getID(username);
                        setLoggedUser(userID);
                        return userID;
                    }
                    else
                    {
                        //Password is Incorrect
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

//Register
int User::registr(string username, string passwd)
{
    //Check if User UsernName is already on Database
    int userExists = getID(username);

    //User not on the Database
    if(userExists == 0)
    {       
        //Connect to Database
        connectDB DB;

        MYSQL* conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

        //Encrypt Password
        string encryptedPasswd = createHash(passwd);

        if(conn)
        {
            stringstream insertQuery;
            insertQuery << "Insert into users(userID, userName, password, isOnline) VALUES (null,'" << username << "','" << encryptedPasswd <<"',0)";

            string queryStr = insertQuery.str();

            //Convert String to Const Char
            const char* q = queryStr.c_str();
            int exeQuery = mysql_query(conn, q);

            if(exeQuery == 0)
            {
                //User Created
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
    else
    {
        //User Already exists;
        return 2;
    }
}

//Get User ID by Username
int User::getID(string username)
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
        selectQuery << "Select * from users where userName ='" << username << "'";

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

//Get User Username by ID
string User::getUsername(int id)
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
         selectQuery << "Select * from users where userID ='" << id << "'";

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
                 string username(chr);
                 return username;
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

//Create User Log Fille With ID
void User::setLoggedUser(int id)
{
    string str_id = to_string(id);
    ofstream f(FILE_DEST, ios::out | ios::trunc);
    f << str_id;
    f.close();
}

//Get User Id on Log Fille
int User::getLoggedUser()
{
    ifstream f(FILE_DEST);

    if (f.is_open())
    {
        vector <string> userData;
        string line = "";

        string data;

        //Get File Lines
        while (getline(f, line))
        {
            userData.push_back(line);
        }

        f.close();

        data = userData[0];

        int userID = stoi(data);
        return userID;
    }
    else
    {
        return 0;
    }
}

//Encrypt Password with Hash
string User::createHash(string passwd)
{
    size_t hashedPasswd;
    string hashedStringPassword;

    // Creating hash
    hash <string> mystdhash;

    hashedPasswd = mystdhash(passwd);

    hashedStringPassword = convertHashToString(hashedPasswd);

    return hashedStringPassword;
}

//Convert Hash to String
string User::convertHashToString(size_t hash)
{
    stringstream ss;

    ss << hash;

    return ss.str();
}

int User::getNumberOfFriends(int userId)
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
         selectQuery << "Select * from user_friends where userID = '" << userId << "' AND accepted = 1 OR friendID = '" << userId << "' AND accepted = 1";

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

int User::getFriends(int userId, int friendRow)
{
    int friendID = 0;

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
         selectQuery << "Select * from user_friends where userID = '" << userId << "' AND accepted = 1 OR friendID = '" << userId << "' AND accepted = 1";

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
                    char *chr1 = row[1];
                    int row0 = convertCharToInt(chr0);
                    int row1 = convertCharToInt(chr1);

                    if(counter == friendRow)
                    {
                        if(row0 == userId)
                        {
                            friendID = row1;
                        }
                        else if(row1 == userId)
                        {
                            friendID = row0;
                        }
                    }

                    counter = counter + 1;
                 }

                 return friendID;

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

int User::getNumberOfFriendRequests(int userId)
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
         selectQuery << "Select * from user_friends where friendID = '" << userId << "' AND accepted = 0";

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

int User::getFriendRequests(int userId, int friendRow)
{
    int friendID = 0;

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
         selectQuery << "Select * from user_friends where friendID = '" << userId << "' AND accepted = 0";

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
                    char *chr1 = row[1];
                    int row0 = convertCharToInt(chr0);
                    int row1 = convertCharToInt(chr1);

                    if(counter == friendRow)
                    {
                        if(row0 == userId)
                        {
                            friendID = row1;
                        }
                        else if(row1 == userId)
                        {
                            friendID = row0;
                        }
                    }

                    counter = counter + 1;
                 }

                 return friendID;

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

int User::getNumberOfGroups(int userId)
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
         selectQuery << "Select * from user_groups where userID = '" << userId << "'";

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

int User::getGroups(int userId, int groupRow)
{
    int groupID = 0;

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
         selectQuery << "Select * from user_groups where userID = '" << userId << "'";

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
                    char *chr1 = row[1];
                    int row0 = convertCharToInt(chr0);
                    int row1 = convertCharToInt(chr1);

                    if(counter == groupRow)
                    {
                        if(row0 == userId)
                        {
                            groupID = row1;
                        }
                        else if(row1 == userId)
                        {
                            groupID = row0;
                        }
                    }

                    counter = counter + 1;
                 }

                 return groupID;

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

int User::checkFriendShip(int userId,int friendID)
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
            selectQuery << "Select * from user_friends where userID ='" << userId << "' AND friendID ='" << friendID <<"' OR userID ='"<<friendID<<"' AND friendID ='"<< userId <<"'";

            string queryStr = selectQuery.str();

            //Convert String to Const Char
            const char* q = queryStr.c_str();

            int exeQuery = mysql_query(conn, q);

            if(!exeQuery)
            {
                res = mysql_store_result(conn);

                int num_rows = mysql_num_rows(res);
                row = mysql_fetch_row(res);

                if(num_rows != 0)
                {
                    char *chr = row[2];
                    int acceptedRequest = convertCharToInt(chr);

                    if(acceptedRequest != 0)
                    {
                        return 1;
                    }
                    else
                    {
                        return 2;
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

int User::sendFriendRequest(int userId,int friendId)
{
    int checkFS = checkFriendShip(userId,friendId);

    if(checkFS == 0)
    {

        //Connect to Database
        connectDB DB;

        MYSQL* conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

        if(conn)
        {
            stringstream insertQuery;
            insertQuery << "Insert into user_friends(userID, friendID, accepted, chatID) VALUES ('"<< userId <<"','" << friendId << "',0,null)";

            string queryStr = insertQuery.str();

            //Convert String to Const Char
            const char* q = queryStr.c_str();
            int exeQuery = mysql_query(conn, q);

            if(exeQuery == 0)
            {
                //Friend Request Sent
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
    else if(checkFS == 2)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}

int User::removeFriend(int userID, int friendID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Delete from user_friends where userID = '"<< userID <<"' AND friendID = '" << friendID << "' OR userID = '"<< friendID <<"' AND friendID = '"<< userID <<"'";

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

int User::acceptFriendRequest(int userID, int friendID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "UPDATE user_friends SET accepted = 1 where userID = '"<< userID <<"' AND friendID = '" << friendID << "' OR userID = '"<< friendID <<"' AND friendID = '"<< userID <<"'";

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

int User::declineFriendRequest(int userID, int friendID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "Delete from user_friends where userID = '"<< userID <<"' AND friendID = '" << friendID << "' OR userID = '"<< friendID <<"' AND friendID = '"<< userID <<"'";

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

int User::addFriendChat(int userID, int friendID, int chatID)
{
    //Connect to Database
    connectDB DB;

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, 0, 0);

    if(conn)
    {
        stringstream insertQuery;
        insertQuery << "UPDATE user_friends SET chatID = '"<< chatID <<"' where userID = '"<< userID <<"' AND friendID = '" << friendID << "' OR userID = '"<< friendID <<"' AND friendID = '"<< userID <<"'";

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

int User::checkGroupPermissions(int userID, int groupID)
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
            selectQuery << "Select * from user_groups where userID ='" << userID << "' AND groupID ='" << groupID << "'";

            string queryStr = selectQuery.str();

            //Convert String to Const Char
            const char* q = queryStr.c_str();

            int exeQuery = mysql_query(conn, q);

            if(!exeQuery)
            {
                //User Found
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);

                if (row != nullptr)
                {
                    //Return User Role on Group
                    char *chr = row[2];
                    int role = convertCharToInt(chr);
                    return role;
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

int User::checkUserInGroup(int userID, int groupID)
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
            selectQuery << "Select * from user_groups where userID ='" << userID << "' AND groupID ='" << groupID << "'";

            string queryStr = selectQuery.str();

            //Convert String to Const Char
            const char* q = queryStr.c_str();

            int exeQuery = mysql_query(conn, q);

            if(!exeQuery)
            {
                //User Found
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);

                if (row != nullptr)
                {
                    cout << row[0];
                    cout << row[1];
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

    mysql_close(conn);
}

int User::getChatID(int userID, int friendID)
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
        selectQuery << "Select * from user_friends where userID = '"<< userID <<"' AND friendID = '" << friendID << "' OR userID = '"<< friendID <<"' AND friendID = '"<< userID <<"'";

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
                char *chr = row[3];
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
