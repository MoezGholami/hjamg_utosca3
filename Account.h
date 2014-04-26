#ifndef __Moez_Hadis_Account
#define __Moez_Hadis_Account

#include "Nosy.h"
#include "Benefector.h"
#include <queue>
#include <pthread.h>
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

#define Lock(m)		while(pthread_mutex_trylock(&m))
#define UnLock(m)	while(pthread_mutex_unlock(&m))

extern ostream out;
class AccountGenerator;

class Account
{
	public:
		~Account(void);
		void cancel(void);


		inline int getID(void);
		inline bool isCancelling(void);
		void oneAct(void);

		// does not create log, will not block the caller, insert into the queues, these methodes are critical
		inline void wait4Charity(Benefector*);
		inline void wait4Watching(Nosy*);

		friend class AccountGenerator;

	private:
		Account(const string &, const string &, int, pthread_t, pthread_mutex_t);

		int ID;
		int val;
		int qcounter;
		string Name;
		string PhoneNumber;

		bool cancelling;

		queue<Nosy*> watchers;
		queue<Benefector*> bens;

		pthread_mutex_t watchQmtx;
		pthread_mutex_t bensQmtx;
		pthread_mutex_t runningMutex;

		pthread_t runningOn;
		pthread_cond_t wakeupCond;
};

void* RunAnAccount(void* acptr);

#endif
