#include "Benefector.h"
#include "Account.h"

void Benefector::help(Account* account)
{
	static int i = 0;
	cerr<<"Benefector helped "<< account->getName() << " help number = " <<++i<<endl;
}


void Benefector::tryToHelp(Account* account)
{
	cerr<<"A benefector is trying to help "<<account->getName()<<endl;
	account->wait4Charity(this);
}
