#pragma once
#include "User.h"
#include "SQLiteService.h"
#include <regex>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2222


UserChoice Menu()
{
	cout << "1 - Sign up\n2 - Sign In\n3 - Start chating\n4 - Exit" << endl;

	int choice{};

	cout << "Enter choice: ";
	cin >> choice;

	while (choice < 1 || choice > 4)
	{
		cout << "Enter choice: ";
		cin >> choice;
	}

	return static_cast<UserChoice>(choice);
}

User* SignUp()
{
	SQLiteService::OpenDatabase();
	SQLiteService::CreateTable();

	cin.ignore();
	cin.clear();

	string name, password;

	std::regex pattern("[\\d\\s]+");
	std::regex spaces_pattern("\\s+");

	bool ifUserExists = false;

	do
	{
		do
		{
			cout << "Enter name: ";
			getline(cin, name);

		} while (std::regex_search(name, pattern));

		do
		{
			cout << "Enter password: ";
			getline(cin, password);

		} while (std::regex_search(password, spaces_pattern));

		ifUserExists = SQLiteService::CheckUser(name);

	} while (ifUserExists);

	SQLiteService::InsertUser(name, password);

	SQLiteService::CloseDatabase();

	return User::GetInstance(name, password, SERVER_IP, SERVER_PORT);

}

User* SignIn()
{
	SQLiteService::OpenDatabase();
	SQLiteService::CreateTable();

	cin.ignore();
	cin.clear();

	string name, password;

	std::regex pattern("[\\d\\s]+");
	std::regex spaces_pattern("\\s+");

	bool ifUserExists = false;

	do
	{
		do
		{
			cout << "Enter name: ";
			getline(cin, name);

		} while (std::regex_search(name, pattern));

		do
		{
			cout << "Enter password: ";
			getline(cin, password);

		} while (std::regex_search(password, spaces_pattern));

		ifUserExists = SQLiteService::CheckUser(name, password);

	} while (!ifUserExists);

	SQLiteService::CloseDatabase();

	return User::GetInstance(name, password, SERVER_IP, SERVER_PORT);
}
