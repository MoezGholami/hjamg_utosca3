#ifndef __Moez_Hadis_BENEFECTOR
#define __Moez_Hadis_BENEFECTOR

#include "Bank.h"
#include "Account.h"
#include <string>
#include <vector>
#include "Threading.h"
using namespace std;

class Benefector
{
	public:
		friend class BenefectorGenerator;

		vector <Account*> helpAccs;
		
		void help(Account* account);
		void destruct(void);
		~Benefector();

		bool isCancelling(void);
		void SetThreadFinished(void);
		
	private:
		Benefector(int, string, Bank*,pthread_t, vector <int>);

		int id;
		string name;
		Bank* benBank;

		pthread_t runningBen;
		pthread_mutex_t helpMtx;

		bool alreadyDestructed;
		bool cancelling;
		bool canBeCancelled;
		bool finished;
};

class BenefectorGenerator
{
	public:
		BenefectorGenerator(Bank*);
		~BenefectorGenerator(void);
		void generateNewBenefector(string name, const vector<int>& AccIDs);
		void Close(void);
	private:
		int idgen;
		Bank* bank;
		bool alreadyClosed;
		vector<Benefector*> GoodMen;
};


class BenefectorConstructEx
	: public Exception
{
	public:
		int Code(void) const;
		const char *Declaration(void) const;
};

void* RunBen (void*);

#endif
