#ifndef __Moez_Hadis_Bank
#define __Moez_Hadis_Bank

#include <vector>
#include "Account.h"
#include "Exception.h"

class AccountGenerator
{
	public:
		AccountGenerator(void);
		Account newAccount(const string &name, const string &phoneNumber);
	private:
		int idcounter;
};

class AccountConstructEx
	: public Exception
{
	public:
		int code(void);
		const char *Declaration(void);
};

#endif
