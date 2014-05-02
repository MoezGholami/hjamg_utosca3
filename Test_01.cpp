#include "Bank.h"
#include "Benefector.h"
#include "Nosy.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	cerr<<"main thread started"<<endl;
	vector <int> ids;
	ids.push_back(1);

	Bank bank;
	BenefectorGenerator ben(&bank);

	Account* a = bank.newAccount("ali","123456");
	Benefector* b = ben.newBenefector("billgates",ids);

	/*	
	Benefector* newBenefector(string name, const vector<int>& AccIDs);
	Account* aliAccount = bank.newAccount("ali", "123456");
	Benefector billgates;
	billgates.tryToHelp(aliAccount);
	 */
}
