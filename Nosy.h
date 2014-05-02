#ifndef __Moez_Hadis_NOSY
#define __Moez_HAdis_NOSY

#include <vector>
#include "pthread.h"
#include "Account.h"
#include "Exception.h"
#include "Bank.h"

class Nosy
{
	public:
		friend class NosyGenerator;
		void nosyWatch(Account* account);
		//vector <Account*> watchAccs;
		~Nosy();
	private:
		int id;
		string name;
		Nosy(int, string, Bank*,pthread_t, vector <int>);
		Bank* bank;
		pthread_t runningNosy;
};

class NosyGenerator
{
	public:
		Nosy* newNosy(int, string, Bank*, vector <int>);
	private:
};

class NosyConstructEx
	: public Exception
{
	public:
		int code(void);
		const char *Declaration(void);
};

void* RunNosy(void* n);

#endif
