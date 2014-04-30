#include "Nosy.h"
#include "Account.h"

void Nosy::nosyWatch(Account* account)
{
	static int i = 0;
	cout<<"Nosy watched"<< account->getName() << " watch number = " <<++i<<endl;
}
