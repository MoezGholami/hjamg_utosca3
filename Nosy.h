#ifndef __Moez_Hadis_NOSY
#define __Moez_HAdis_NOSY

#include <vector>
#include <pthread.h>
#include "Account.h"
#include "Exception.h"
#include "Bank.h"
#include <fstream>
#include "Threading.h"
using namespace std;

class Nosy
{
	public:
		friend class NosyGenerator;

		vector <Account*> watchAccs;

		void nosyWatch(Account* account);
		void destruct(void);
		~Nosy(void);

		bool isCancelling(void);
		void ClearCanBeCancelled(void);
		void SetThreadFinished(void);

		string getName(void);

	private:
		Nosy(int, string, Bank*,pthread_t, const vector <int>&);

		int id;
		string name;
		Bank* nosyBank;

		ofstream out;

		pthread_t runningNosy;
		pthread_mutex_t watchMtx;

		bool alreadyDestructed;
		bool cancelling;
		bool canBeCancelled;
		bool finished;
};

class NosyGenerator
{
	public:
		NosyGenerator(Bank*);
		~NosyGenerator(void);
		void generateNewNosy(string, const vector <int>&);

		void Close(void);
	private:
		int idgen;
		Bank* bank;
		bool alreadyClosed;
		vector<Nosy*> agents;
};

class NosyConstructEx
	: public Exception
{
	public:
		int Code(void) const;
		const char *Declaration(void) const;
};

void* RunNosy(void* n);

#endif
