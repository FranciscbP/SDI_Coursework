#include "headers/user.h"
#include "headers/connectdb.h"
#include "headers/otherFunctions.h"

const string FILE_DEST = "log.txt";

User::User()
{

}

//Login
int User::login(string email,string passwd)
{
    //Connect to Database
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
                if(row[2] == passwd)
                {
                    //Password is Correct
                    int userID = getID(email);
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
}

//Register
int User::registr(string email, string passwd)
{
    //Check if User Email is already on Database
    int userExists = getID(email);

    if(userExists == 0)
    {
        //User not on the Database

        //Connect to Database
        connectDB DB;

        MYSQL* conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, NULL, 0);

        if(conn)
        {
            stringstream insertQuery;
            insertQuery << "INSERT INTO users(userID, email, password) VALUES ('NULL','" << email << "','" << passwd <<"')";

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
    }
    else
    {
        //User Already exists;
        return 2;
    }


}

//Get User ID by Email
int User::getID(string email)
{
    //Connect to Database
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
}

//Get User Email by ID
string User::getEmail(int id)
{
    //Connect to Database
    connectDB DB;

     MYSQL* conn;
     MYSQL_ROW row;
     MYSQL_RES* res;
     conn = mysql_init(0);
     conn = mysql_real_connect(conn, DB.server, DB.user, DB.passwd, DB.db, 3306, NULL, 0);

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
                 //Return Email
                 char *chr = row[1];
                 string email(chr);
                 return email;
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
