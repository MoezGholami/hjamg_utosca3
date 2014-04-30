#include "Account.h"
#include "Nosy.h"
#include "Benefector.h"

Account::Account(const string &name, const string &phoneNumber, int id, pthread_t rt, pthread_mutex_t rm)
{
	Name=name;
	PhoneNumber=phoneNumber;

	ID=id;
	pthread_mutex_init(&runningMutex, 0);
	pthread_mutex_init(&bensQmtx, 0);
	pthread_mutex_init(&watchQmtx, 0);
	pthread_cond_init(&wakeupCond,0);
	cancelling=false;
	Lock(runningMutex);
	qcounter=0;
	val=0;
	runningOn=rt;
	runningMutex=rm;
}

Account::~Account(void)
{
	cancel();
}

void Account::cancel(void)
{
	cerr<<"Account with ID "<< ID << " for "<<Name<<" has been closed."<<endl;
	cancelling=true;
	pthread_join(runningOn, 0);
	pthread_cond_destroy(&wakeupCond);
	pthread_mutex_destroy(&watchQmtx);
	pthread_mutex_destroy(&bensQmtx);
	UnLock(runningMutex);
	pthread_mutex_destroy(&runningMutex);
}

int Account::getID(void)
{
	return ID;
}

string Account::getName()
{
	return Name;
}

bool Account::isCancelling(void)
{
	return cancelling;
}

void Account::wait4Charity(Benefector* christ)
{
	Lock(bensQmtx);
	bens.push(christ);
	cerr<<"someone is trying to help me, i am "<<getName()<<endl;
	pthread_cond_broadcast(&wakeupCond); //TODO: search if we are awake what will happern if another person wake us up?
	UnLock(bensQmtx);
}

void Account::wait4Watching(Nosy* n)
{
	Lock(watchQmtx);
	watchers.push(n);
	pthread_cond_broadcast(&wakeupCond);
	UnLock(watchQmtx);
}

void Account::oneAct(void)
{
	cerr<<"oneAct() has been executed.\n";
	Lock(watchQmtx);
	Lock(bensQmtx);

	qcounter=(qcounter+1)%6;
	if(qcounter)	//access for a nosy
	{
		if(watchers.size()>0)
		{
			Nosy *n=watchers.front();
			n->nosyWatch(this);
			watchers.pop();
		}
	}
	else		//access for a benefector
	{
		if(bens.size()>0)
		{
			Benefector *b=bens.front();
			b->help(this);
			bens.pop();
		}
	}
	if(bens.size()==0 && watchers.size()==0)
	{
		UnLock(watchQmtx);
		UnLock(bensQmtx);
		while(pthread_cond_wait(&wakeupCond,&runningMutex)); //Block
	}
	else
	{
		UnLock(watchQmtx);
		UnLock(bensQmtx);
	}
}

void *RunAnAccount(void* acptr)
{
	 /*
	string a = "";
	for (int i = 0 ; i < 50000 ; i ++)
		a+= "aaa";
	// */
	cerr<<"RunAnAccount() has been executed.\n";
	Account *inUse=(Account *)acptr;
	cerr<<"is cancelling "<<inUse->isCancelling()<<endl;
	while(!inUse->isCancelling())
	{
		cerr<<"entering one act\n";
		Account *inUse=(Account *)acptr;
		inUse->oneAct();
	}
	cerr<<"has been joined"<<endl;
	return 0;
}
