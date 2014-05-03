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
		void Close(void);

		Account* newAccount(const string &name, const string &phoneNumber);
		vector<Account*> getAccountsByIDs(const vector<int>& IDs);
		Account* getAccountByID(int id);
	private:
		vector <Account*> accs;
		map<int, Account*> idAccsMap;

		bool alreadyClosed;
};

class AccountConstructEx
	: public Exception
{
	public:
		int Code(void) const;
		const char *Declaration(void) const;
};

#endif
