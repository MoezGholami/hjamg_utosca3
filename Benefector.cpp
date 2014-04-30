#include "Benefector.h"
#include "Account.h"

void Benefector::help(Account* account)
{
	static int i = 0;
	cout<<"Benefector helped "<< account->getName() << " help number = " <<++i<<endl;
}


void Benefector::tryToHelp(Account* account)
{
	account->wait4Charity(this);
}
