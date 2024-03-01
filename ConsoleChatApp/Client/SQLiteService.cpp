#include "SQLiteService.h"
#include <exception>
#include <iostream>

using std::cout;
using std::endl;

using std::exception;

bool SQLiteService::OpenDatabase()
{

    if (sqlite3_open(database_name.c_str(), &db))
    {
        throw exception(sqlite3_errmsg(db));
        return false;
    }

    return true;
}

bool SQLiteService::CreateTable()
{
    char* errMsg = nullptr;

    const char* sql_create_table =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "password TEXT NOT NULL"
        ");";

    if ((sqlite3_exec(db, sql_create_table, 0, 0, &errMsg)) != SQLITE_OK)
    {
        throw exception(errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool SQLiteService::InsertUser(string name, string password)
{
    if (!CheckUser(name))
    {
        char* errMsg = nullptr;

        std::string sql_insert_data = "INSERT INTO users (name, password) VALUES ('" + name + "', '" + password + "');";

        if ((sqlite3_exec(db, sql_insert_data.c_str(), 0, 0, &errMsg)) != SQLITE_OK)
        {
            throw exception(errMsg);
            return false;
        }

        return true;
    }

    return false;
}

bool SQLiteService::CheckUser(string name)
{
    sqlite3_stmt* stmt;

    string sql_check = "SELECT * FROM users WHERE name='" + name + "';";

    if ((sqlite3_prepare_v2(db, sql_check.c_str(), -1, &stmt, NULL)) != SQLITE_OK)
    {
        throw exception("error in  checkuser()");
        return false;
    }

    if ((sqlite3_step(stmt)) != SQLITE_ROW)
    {
        return false;
    }

    sqlite3_finalize(stmt);

    return true;
}

bool SQLiteService::CheckUser(string name, string password)
{
    sqlite3_stmt* stmt;

    string sql_check = "SELECT * FROM users WHERE name='" + name + "' AND password='" + password + "';";

    if ((sqlite3_prepare_v2(db, sql_check.c_str(), -1, &stmt, NULL)) != SQLITE_OK)
    {
        throw exception("error in  checkuser()");
        return false;
    }

    if ((sqlite3_step(stmt)) != SQLITE_ROW)
    {
        return false;
    }

    sqlite3_finalize(stmt);

    return true;
}

bool SQLiteService::ShowUsers()
{
    sqlite3_stmt* stmt;
    string sql_select = "SELECT * FROM users;";

    if ((sqlite3_prepare_v2(db, sql_select.c_str(), -1, &stmt, NULL)) != SQLITE_OK)
    {
        throw exception("error in ShowUsers()");
        return false;
    }

    cout << "Список користувачів:" << endl;
    while ((sqlite3_step(stmt)) == SQLITE_ROW)
    {
        string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        cout << "Ім'я: " << name << endl;
    }

    sqlite3_finalize(stmt);

    return true;
}

bool SQLiteService::CloseDatabase()
{
    sqlite3_close(db);
    return true;
}


string SQLiteService::database_name = "chatapp";
sqlite3* SQLiteService::db = nullptr;
