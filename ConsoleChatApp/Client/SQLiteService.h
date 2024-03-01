#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>

using std::string;

class SQLiteService
{

    static string database_name;
    static sqlite3* db;

public:

    static bool OpenDatabase();

    static bool CreateTable();

    static bool InsertUser(string name, string password);

    static bool CheckUser(string name);

    static bool CheckUser(string name, string password);

    static bool ShowUsers();

    static bool CloseDatabase();

};

