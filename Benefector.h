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
		void help(Account* account);
		void tryToHelp(Account* account);
		void destruct(void);
	private:
};

class BenefectorGenerator
{
	public:
		BenefectorGenerator(Bank*);
		Benefector* newBenefector(string name, const vector<int>& AccIDs);
		void Close(void);
};

#endif
