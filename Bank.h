#ifndef __Moez_Hadis_Bank
#define __Moez_Hadis_Bank

#include <vector>
#include <map>
#include "Account.h"
#include "Exception.h"

class Bank
{
	public:
		Bank(void);
		~Bank(void);
		Account* newAccount(const string &name, const string &phoneNumber);
		vector<Account*> getAccountsByIDs(const vector<int>& IDs);
	private:
		vector <Account*> accs;
		map<int, Account*> idAccsMap;
};

class AccountConstructEx
	: public Exception
{
	public:
		int code(void);
		const char *Declaration(void);
};

#endif
