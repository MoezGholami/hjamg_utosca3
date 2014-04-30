#ifndef __Moez_Hadis_Bank
#define __Moez_Hadis_Bank

#include <vector>
#include "Account.h"
#include "Exception.h"

class Bank
{
	public:
		Bank(void);
		~Bank(void);
		Account* newAccount(const string &name, const string &phoneNumber);
	private:
		vector <Account*> accs;
};

class AccountConstructEx
	: public Exception
{
	public:
		int code(void);
		const char *Declaration(void);
};

#endif
