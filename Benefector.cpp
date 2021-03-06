#include "Benefector.h"
#include <string>
#include <sstream>

Benefector::Benefector(int i, string n, Bank* b ,pthread_t rn, vector <int> p)
{
	id = i;
	name = n;
	benBank = b;
	runningBen = rn;
	helpAccs = benBank->getAccountsByIDs(p);
	pthread_mutex_init(&helpMtx, 0);
	alreadyDestructed = false;
	cancelling=false;
	canBeCancelled=false;
	finished=false;

	stringstream ss;
	ss<<"Benefector: "<<name<<" with ID: "<<id<<" has been created and helps: ";
	for (unsigned i = 0 ; i < helpAccs.size() ; i++)
		ss<<helpAccs[i]->getName()<<" ";
	ss<<endl;
	cout<<ss.str();
}

Benefector::~Benefector(void)
{
	destruct();
}

void Benefector::destruct(void)
{
	if(alreadyDestructed)
		return ;
	alreadyDestructed=true;
	while(!canBeCancelled)
		usleep(10000);
	cancelling=true;
	if(!finished)
		usleep(10000);
	
	pthread_mutex_destroy(&helpMtx);
}

void Benefector::help(Account* account)
{
	if (alreadyDestructed)
		return;
	Lock(helpMtx);
	canBeCancelled = false;

	int val = account->IncAndReturn(0);

	stringstream ss;

	if(val<1000)
	{
		val = account->IncAndReturn(10);
		ss<<"Benefector with name: "<<name<<" with ID: "<<id<<" helped "<<account->getName()<<" with ID: "<<account->getID()<<" and final value is: "<<val<<endl;
	}
	else
		ss<<"Benefector with name: "<<name<<" with ID: "<<id<<" did not help "<<account->getName()<<" with ID: "<<account->getID()<<" bacause value is more than 1000 and final value is: "<<val<<endl;

	cout<<ss.str();	

	canBeCancelled = true;
	UnLock(helpMtx);
}


bool Benefector::isCancelling(void)
{
	return cancelling;
}

BenefectorGenerator::BenefectorGenerator(Bank* b)
{
	idgen = 0;
	bank = b;
	alreadyClosed=false;

}

BenefectorGenerator::~BenefectorGenerator(void)
{
	Close();
}

void Benefector::SetThreadFinished(void)
{
	finished=true;
}

void BenefectorGenerator::generateNewBenefector(string name, const vector<int>& AccIDs)
{
	pthread_t rn;
	Benefector* ben = new Benefector(++idgen,name,bank,rn,AccIDs);
	if (pthread_create(&rn, 0, RunBen, (void*) ben))
		throw BenefectorConstructEx();
	GoodMen.push_back(ben);
}

void BenefectorGenerator::Close(void)
{
	if(alreadyClosed)
		return ;
	alreadyClosed=true;
}

string Benefector::getName(void)
{
	return name;
}

int BenefectorConstructEx::Code(void) const
{
	return 3;	
}

const char* BenefectorConstructEx::Declaration(void) const
{
	return "An error occured while creating a new benefector.\n";
}

void* RunBen(void* acptr)
{
	Benefector* ben = (Benefector*) acptr;
	for (unsigned i = 0 ; !ben->isCancelling() && i < ben->helpAccs.size() ; i=(i+1))//%ben->helpAccs.size())
		ben->helpAccs[i]->wait4Charity(ben);
	ben->SetThreadFinished();
	return 0;	
}
