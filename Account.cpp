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
	out.open(string(name+".txt").c_str(), ios::out);
	out<<"log of Qs for "<<name<<"'s account:\n";

	stringstream ss;
	ss<<"Acount: "<<Name<<" with ID: "<<ID<<" has been created.\n";
	cout<<ss.str();
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
	cancelling=true;
	pthread_cond_broadcast(&wakeupCond);
	while(!finished)
		usleep(10000);
	UnLock(runningMutex);
	pthread_cond_destroy(&wakeupCond);
	pthread_mutex_destroy(&watchQmtx);
	pthread_mutex_destroy(&bensQmtx);
	pthread_mutex_destroy(&valMtx);
	pthread_mutex_destroy(&runningMutex);

	stringstream ss;
	ss<<"Acount: "<<Name<<" with ID: "<<ID<<" is exiting.\n";
	cout<<ss.str();
}

void Account::printQs(void)
{
	out<<"\n\n\nBenefector Q:\t";
	for(unsigned i=0; i<bens.size(); ++i)
		out<<bens[i]->getName()<<", ";
	out<<"\nWatchers Q:\t";
	for(unsigned i=0; i<watchers.size(); ++i)
		out<<watchers[i]->getName()<<", ";
	out<<endl;
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

	stringstream ss;
	if (plus == 0)
		ss<<"The value of "<<Name<<" has not been changed and it is: "<<v<<endl;
	else
		ss<<"The value of "<<Name<<" has been changed and it is: "<<v<<endl;
	cout<<ss.str();
	
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

	pthread_cond_broadcast(&wakeupCond);
	UnLock(bensQmtx);
}

void Account::wait4Watching(Nosy* n)
{
	if(destructorCalled)
		return ;
	canBeCancelled=false;
	Lock(watchQmtx);
	watchers.push_back(n);

	pthread_cond_broadcast(&wakeupCond);
	UnLock(watchQmtx);
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

	printQs();
	qcounter=(qcounter+1)%6;

	if(qcounter>0 && watchers.size()>0)	//access for a nosy
	{
		Nosy *n=watchers.front();
		n->nosyWatch(this);
		out<<"Picking "<<n->getName()<<"\n";
		watchers.pop_front();
	}
	else		//access for a benefector
	{
		if(watchers.size()==0)	//if there is no watcher and you have to serve a benefector, then start again looking for watchers
			qcounter=0;
		if(bens.size()>0)
		{
			Benefector *b=bens.front();
			b->help(this);
			out<<"Picking "<<b->getName()<<"\n";
			bens.pop_front();
		}
	}
	if(bens.size()==0 && watchers.size()==0)
	{
		UnLock(watchQmtx);
		UnLock(bensQmtx);
		canBeCancelled=destructorCalled;
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
	Account *inUse=(Account *)acptr;

	while(!inUse->isCancelling())
	{
		Account *inUse=(Account *)acptr;
		inUse->oneAct();
	}
	inUse->SetThreadFinished();
	return 0;
}
