#include "Bank.h"
#include "Benefector.h"
#include "Nosy.h"
#include <iostream>

using namespace std;

int main()
{
	cout<<"Hello world!"<<endl;
	AccountGenerator bank;
	Account aliAccount = bank.newAccount("ali", "123456");
	Benefector billgates;
	billgates.tryToHelp(&aliAccount);
}
