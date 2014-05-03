#include "Account.h"
#include "Nosy.h"
#include "Benefector.h"
#include <sstream>
#include <string>

Account::Account(const string &name, const string &phoneNumber, int id, pthread_t rt, pthread_mutex_t rm)
{
	Name=name;
	PhoneNumber=phoneNumber;

	ID=id;
	pthread_mutex_init(&runningMutex, 0);
	pthread_mutex_init(&bensQmtx, 0);
	pthread_mutex_init(&watchQmtx, 0);
	pthread_mutex_init(&valMtx, 0);
	pthread_cond_init(&wakeupCond,0);
	cancelling=false;
	canBeCancelled=true;
	destructorCalled=false;
	finished=false;
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
	if(finished)
		return ;
	destructorCalled=true;
	if(!canBeCancelled)
		usleep(10000);
	stringstream ss;
	ss<<"Account with ID "<< ID << " for "<<Name<<" has been closed."<<endl;
	cerr<<ss.str();
	cancelling=true;
	pthread_cond_broadcast(&wakeupCond);
	while(!finished)
		usleep(10000);
	//pthread_join(runningOn, 0);
	UnLock(runningMutex);
	pthread_cond_destroy(&wakeupCond);
	pthread_mutex_destroy(&watchQmtx);
	pthread_mutex_destroy(&bensQmtx);
	pthread_mutex_destroy(&valMtx);
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

int Account::IncAndReturn(int plus)
{
	Lock(valMtx);
	val+=plus;
	int v=val;
	UnLock(valMtx);
	return v;
}

void Account::wait4Charity(Benefector* christ)
{
	if(destructorCalled)
		return ;
	canBeCancelled=false;

	Lock(bensQmtx);
	bens.push_back(christ);

	cerr<<"wait4charity has been called and gone inside\n";
	stringstream ss;
	ss<<"in wait4charity, in benefector's thread: someone is trying to help me, i am "<<getName()<<endl;
	cerr<<ss.str();

	pthread_cond_broadcast(&wakeupCond);
	UnLock(bensQmtx);
}

void Account::wait4Watching(Nosy* n)
{
	if(destructorCalled)
		return ;
	canBeCancelled=false;
	Lock(watchQmtx);
	//if(watchers.end()==find(watchers.begin(), watchers.end(), n))
		watchers.push_back(n);

	stringstream ss;
	ss<<"in wait4Watching, in nosy's thread: someone is watching me, i am "<<getName()<<endl;
	cerr<<ss.str();

	pthread_cond_broadcast(&wakeupCond);
	UnLock(watchQmtx);

	cerr<<"Unlocked watchQMtx\n";
}

void Account::wakeMeUp(void)
{
	pthread_cond_broadcast(&wakeupCond);
}

bool Account::isWaitingForWatching(Nosy *n)
{
	Lock(watchQmtx);
	bool result=(watchers.end()!=find(watchers.begin(), watchers.end(), n));
	UnLock(watchQmtx);
	return result; 
}

void Account::SetThreadFinished(void)
{
	finished=true;
}

void Account::oneAct(void)
{
	Lock(watchQmtx);
	Lock(bensQmtx);

	stringstream ss;
	ss<<"oneAct() has been executed.\n";
	ss<<"benQsize = "<<bens.size()<<endl;
	ss<<"watchQsize = "<<watchers.size()<<endl;
	cerr<<ss.str();

	qcounter=(qcounter+1)%6;

	ss<<"qcounter: "<<qcounter<<endl;
	cerr<<ss.str();

	if(qcounter>0 && watchers.size()>0)	//access for a nosy
	{
		Nosy *n=watchers.front();
		n->nosyWatch(this);
		watchers.pop_front();
	}
	else		//access for a benefector
	{
		if(watchers.size()==0)	//if there is no watcher and you have to serve a benefector, then start again looking for watchers
			qcounter=0;
		if(bens.size()>0)
		{
			cerr<<"responsing to benefectors\n";
			Benefector *b=bens.front();
			b->help(this);
			bens.pop_front();
		}
	}
	if(bens.size()==0 && watchers.size()==0)
	{
		UnLock(watchQmtx);
		UnLock(bensQmtx);
		canBeCancelled=destructorCalled;
		cerr<<"SSSleeping thread because Qs are empty\n";
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

	stringstream ss;
	ss<<"is cancelling "<<inUse->isCancelling()<<endl;
	cerr<<ss.str();

	while(!inUse->isCancelling())
	{
		cerr<<"entering one act\n";
		Account *inUse=(Account *)acptr;
		inUse->oneAct();
	}
	cerr<<"has been joined"<<endl;
	inUse->SetThreadFinished();
	return 0;
}
