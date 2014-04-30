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
	canBeCancelled=true;
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
	while(!canBeCancelled)
		usleep(100000);
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
	cerr<<"in wait4charity, in benefector's thread: someone is trying to help me, i am "<<getName()<<endl;
	canBeCancelled=false;
	pthread_cond_broadcast(&wakeupCond); //TODO: search if we are awake what will happern if another person wake us up?
	UnLock(bensQmtx);
}

void Account::wait4Watching(Nosy* n)
{
	Lock(watchQmtx);
	watchers.push(n);
	canBeCancelled=false;
	pthread_cond_broadcast(&wakeupCond);
	UnLock(watchQmtx);
}

void Account::oneAct(void)
{
	Lock(watchQmtx);
	Lock(bensQmtx);
	cerr<<"oneAct() has been executed.\n";
	cerr<<"benQsize= "<<bens.size()<<endl;

	qcounter=(qcounter+1)%6;
	if(qcounter>0 && watchers.size()>0)	//access for a nosy
	{
		Nosy *n=watchers.front();
		n->nosyWatch(this);
		watchers.pop();
	}
	else		//access for a benefector
	{
		if(watchers.size()==0)	//if there is no watcher and you have to serve a benefector, then start again looking for watcers
			qcounter=0;
		if(bens.size()>0)
		{
			cerr<<"responsing to benefectors\n";
			Benefector *b=bens.front();
			b->help(this);
			bens.pop();
		}
	}
	if(bens.size()==0 && watchers.size()==0)
	{
		UnLock(watchQmtx);
		UnLock(bensQmtx);
		canBeCancelled=true;
		cerr<<"sleeping thread because Qs are empty\n";
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
