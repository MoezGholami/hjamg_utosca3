#include "Nosy.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Nosy::Nosy(int i, string n, Bank* b, pthread_t rn, const vector<int> &ids)
{
	id = i;
	name = n;
	nosyBank = b;
	runningNosy = rn;
	pthread_mutex_init(&watchMtx, 0);
	watchAccs = nosyBank->getAccountsByIDs(ids);
	out.open(string(n+".txt").c_str(), ios::out);
	alreadyDestructed = false;
	cancelling=false;
	canBeCancelled=false;
	finished=false;
	
	stringstream ss;
	ss<<"Nosy: "<<name<<" with ID: "<<id<<" has been created and watches: ";
	for (unsigned i = 0 ; i < watchAccs.size() ; i++)
		ss<<watchAccs[i]->getName()<<" ";
	ss<<endl;
	cout<<ss.str();
}

void Nosy::destruct(void)
{
	if(alreadyDestructed)
		return;
	alreadyDestructed = true;
	while(!canBeCancelled)
	{
		usleep(10000);
	}
	cancelling=true;
	while(!finished)
	{
		usleep(10000);
	}
	pthread_mutex_destroy(&watchMtx);
	out.close();
}

Nosy::~Nosy()
{
	destruct();
}

string Nosy::getName(void)
{
	return name;
}

bool Nosy::isCancelling(void)
{
	return cancelling;
}

void Nosy::ClearCanBeCancelled(void)
{
	canBeCancelled=false;
}

void Nosy::SetThreadFinished(void)
{
	finished=true;
}

void Nosy::nosyWatch(Account* account)
{
	if(alreadyDestructed)
		return ;
	Lock(watchMtx);
	canBeCancelled=false;
	out<<name<<" with ID: "<<account->getID()<<" watched"<< account->getName() << " remaining: "<<account->IncAndReturn(0) <<endl;

	stringstream ss;
	ss<<"Nosy: "<<name<<" with ID: "<<id<<"has watched "<<account->getName()<<" with ID: "<<account->getID()<<endl;
	cout<<ss.str();

	canBeCancelled=true;
	UnLock(watchMtx);
}

NosyGenerator::NosyGenerator(Bank* b)
{
	idgen = 0;
	bank = b;
	alreadyClosed=false;
}

NosyGenerator::~NosyGenerator(void)
{
	Close();
}

void NosyGenerator::generateNewNosy(string n, const vector<int> &ids)
{
	pthread_t rn;
	Nosy* nosy = new Nosy(++idgen, n, bank, rn, ids);
	if (pthread_create(&rn, 0, RunNosy, (void*)nosy))
		throw NosyConstructEx();
	agents.push_back(nosy);
}

void NosyGenerator::Close(void)
{
	if(alreadyClosed)
		return ;
	alreadyClosed=true;
	for(unsigned i=0; i<agents.size(); ++i)
		(agents[i])->destruct();
}

const char* NosyConstructEx::Declaration(void) const
{
	return "An error occured while creating a new nosy.\n";
}

int NosyConstructEx::Code(void) const
{
	return 2;
}	

void* RunNosy(void* acptr)
{
	Nosy* nosy = (Nosy*) acptr;
	for (unsigned i = 0 ; !nosy->isCancelling() ; i=(i+1)%nosy->watchAccs.size())
	{
		nosy->watchAccs[i]->wakeMeUp();
		if(!(nosy->watchAccs[i]->isWaitingForWatching(nosy)))
		{
			nosy->watchAccs[i]->wait4Watching(nosy);
		}
	}
	nosy->SetThreadFinished();			
	return 0;
}
