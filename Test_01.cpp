#include "Bank.h"
#include "Benefector.h"
#include "Nosy.h"
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;

int main()
{
	cerr<<"main thread started"<<endl;
	vector <int> ids;
	ids.push_back(1);

	Bank bank;
	BenefectorGenerator benefectors(&bank);
	NosyGenerator nosies(&bank);

	bank.newAccount("ali","123456");
	benefectors.generateNewBenefector("billgates",ids);
//	benefectors.generateNewBenefector("dianna",ids);
	nosies.generateNewNosy("fozul1",ids);
//	nosies.generateNewNosy("fozul2",ids);
	usleep(1000000);
	benefectors.Close();
	nosies.Close();
}
