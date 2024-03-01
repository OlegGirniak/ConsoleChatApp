#include "UIFunc.h"


int main()
{

	User* user = nullptr;

	while (true)
	{
		switch (Menu())
		{
		case UserChoice::SignUp:
			user = SignUp();

			break;

		case UserChoice::SignIn:
			user = SignIn();

			break;

		case UserChoice::StartChating:
			if (user != nullptr)
				user->StartChating();
			else
				cout << "You need signup or signin" << endl;
			
			break;

		case UserChoice::Exit:
			delete user;
			user = nullptr;
			return 0;

			break;
		default:
			break;
		}
	}

	return 0;
}



