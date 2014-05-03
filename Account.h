#ifndef __Moez_Hadis_Account
#define __Moez_Hadis_Account

#include <queue>
#include <iostream>
#include <string>
#include "Threading.h"
using namespace std;

class Bank;
class Benefector;
class Nosy;

class Account
{
	public:
		~Account(void);
		void cancel(void);


		int getID(void);
		string getName(void);
		inline bool isCancelling(void);
		void oneAct(void);

		void wait4Charity(Benefector*);
		void wait4Watching(Nosy*);

		int IncAndReturn(int plus);

		friend class Bank;

		void SetThreadFinished(void);

	private:
		Account(const string &, const string &, int, pthread_t, pthread_mutex_t);

		int ID;
		int val;
		int qcounter;
		string Name;
		string PhoneNumber;

		bool cancelling;
		bool canBeCancelled;
		bool destructorCalled;
		bool finished;

		queue<Nosy*> watchers;
		queue<Benefector*> bens;

		pthread_mutex_t watchQmtx;
		pthread_mutex_t bensQmtx;
		pthread_mutex_t runningMutex;
		pthread_mutex_t valMtx;

		pthread_t runningOn;
		pthread_cond_t wakeupCond;
};

void* RunAnAccount(void* acptr);

#endif
