#include "Benefector.h"

Benefector::Benefector(int i, string n, Bank* b ,pthread_t rn, vector <int> p)
{
	id = i;
	name = n;
	benBank = b;
	runningBen = rn;
	helpAccs = benBank->getAccountsByIDs(p);
	alreadyDestructed = false;
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
	//remainder
}


Benefector* BenefectorGenerator::newBenefector(string name, const vector<int>& AccIDs)
{
	pthread_t rn;
	Benefector* ben = new Benefector(++idgen,name,bank,rn,AccIDs);
	if (pthread_create(&rn, 0, RunBen, (void*) ben))
		throw BenefectorConstructEx();
	return ben;
}

void Benefector::help(Account* account)
{
	static int i = 0;
	cerr<<"Benefector helped "<< account->getName() << " help number = " <<++i<<endl;
}


void Benefector::tryToHelp(Account* account)
{
	cerr<<"In try2help: A benefector is trying to help "<<account->getName()<<endl;
	account->wait4Charity(this);
}

BenefectorGenerator::BenefectorGenerator(Bank* b)
{
	idgen = 0;
	bank = b;

}

int BenefectorConstructEx::code(void)
{
	return 3;	
}

const char* BenefectorConstructEx::Declaration(void)
{
	return "An error occured while creating a new benefector.\n";
}

void* RunBen(void* acptr)
{
	Benefector* ben = (Benefector*) acptr;
	for (unsigned i = 0 ; i < ben->helpAccs.size() ; i++)
		ben->helpAccs[i]->wait4Charity(ben);
	return 0;	
}
