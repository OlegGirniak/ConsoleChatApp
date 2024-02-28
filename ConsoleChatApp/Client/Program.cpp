#include "User.h"
#include <regex>

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
	cin.ignore();
	cin.clear();

	string name, password;

	std::regex pattern("[\\d\\s]+");
	std::regex spaces_pattern("\\s+");

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

	// to do sql

	return User::GetInstance(name, password, "127.0.0.1", 2222);

}

User* SignIn()
{
	string name, password;

	std::regex pattern("[\\d\\s]+");
	std::regex spaces_pattern("\\s+");

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

	// to do sql 

	return nullptr;
}



int main()
{
	User* user = nullptr;


	while (true)
	{
		switch (Menu())
		{
		case UserChoice::SignUp:
			system("cls");
			user = SignUp();
			system("cls");
			cout << "Sign up success" << endl;
			break;

		case UserChoice::SignIn:
			system("cls");
			user = SignIn();
			system("cls");
			break;

		case UserChoice::StartChating:
			if (user != nullptr)
			{
				system("cls");
				user->StartChating();
			}
			else
			{
				system("cls");
				cout << "Sign in or Sign up" << endl;
			}
			break;

		case UserChoice::Exit:
			if (user != nullptr) 
			{
				delete user;
				user = nullptr;
			}
			return 0;
			break;

		default:
			break;
		}
	}

	return 0;
}