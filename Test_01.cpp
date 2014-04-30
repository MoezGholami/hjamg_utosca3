#include "Bank.h"
#include "Benefector.h"
#include "Nosy.h"
#include <iostream>

using namespace std;

int main()
{
	cerr<<"main thread started"<<endl;
	Bank bank;
	Account* aliAccount = bank.newAccount("ali", "123456");
	Benefector billgates;
	billgates.tryToHelp(aliAccount);
}
