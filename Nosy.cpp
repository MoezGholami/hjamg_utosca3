#include "Nosy.h"

void Nosy::nosyWatch(Account* account)
{
	static int i = 0;
	cerr<<"Nosy watched"<< account->getName() << " watch number = " <<++i<<endl;
}
