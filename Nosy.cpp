#include "Nosy.h"
#include <iostream>
#include <fstream>

Nosy::Nosy(int i, string n, Bank* b, pthread_t rn, vector <int> ids)
{
	id = i;
	name = n;
	bank = b;
	ofstream file;
	string filename = name + ".txt";
	file.open(filename.c_str());
	file.close();
	runningNosy = rn;
//	watchAccs = bank->getAccountsByIDs(ids);
}

Nosy::~Nosy()
{
//	pthread_join(runningNosy,0);
}

void Nosy::nosyWatch(Account* account)
{
	ofstream file;
	string filename = name + ".txt";
	file.open(filename.c_str());
	file<<name<<" with id"<<id<<" watched"<< account->getName() << " remaining: "<<account->IncAndReturn(0) <<endl;
	file.close();
}

Nosy* NosyGenerator::newNosy(int i, string n, Bank* bank, vector<int> ids)
{
	pthread_t rn;
	Nosy* nosy = new Nosy(i, n, bank, rn, ids);
	if (pthread_create(&rn, 0, RunNosy, (void*)nosy))
		throw NosyConstructEx();
	return nosy;
}

const char* NosyConstructEx::Declaration(void)
{
	return "An error occured while creating a new nosy.\n";
}

int NosyConstructEx::code(void)
{
	return 2;
}	

void* RunNosy(void* acptr)
{
//	Nosy* nosy = (Nosy*) acptr;
	//TODO is cacelling
//	for (unsigned i = 0 ; i < nosy->watchAccs.size() ; i++)
//		nosy->watchAccs[i]->wait4Watching(nosy);
			
	return 0;
}
