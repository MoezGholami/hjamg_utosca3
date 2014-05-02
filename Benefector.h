#ifndef __Moez_Hadis_BENEFECTOR
#define __Moez_Hadis_BENEFECTOR

#include "Bank.h"
#include "Account.h"
#include <string>
#include <vector>
using namespace std;

class Benefector
{
	public:
		friend class BenefectorGenerator;
		void help(Account* account);
		void tryToHelp(Account* account);
		void destruct(void);
		vector <Account*> helpAccs;
		~Benefector();
	private:
		int id;
		string name;
		Benefector(int, string, Bank*,pthread_t, vector <int>);
		Bank* benBank;
		pthread_t runningBen;
		bool alreadyDestructed;
};

class BenefectorGenerator
{
	public:
		BenefectorGenerator(Bank*);
		Benefector* newBenefector(string name, const vector<int>& AccIDs);
		void Close(void);
	private:
		int idgen;
		Bank* bank;
};


class BenefectorConstructEx
	: public Exception
{
	public:
		int code(void);
		const char *Declaration(void);
};

void* RunBen (void*);

#endif
