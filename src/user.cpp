#include "headers/user.h"
#include "headers/connectdb.h"

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
        stringstream querySelect;
        querySelect << "Select * from users where email ='" << email << "'";

        string queryStr = querySelect.str();
        const char* q = queryStr.c_str();

        int exeQuery = mysql_query(conn, q);

        if(!exeQuery)
        {
            cout << "The query executed successfully with no error." << endl;

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
            cout << "Error message: " << mysql_error(conn) <<":" <<mysql_errno(conn) << endl;
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
