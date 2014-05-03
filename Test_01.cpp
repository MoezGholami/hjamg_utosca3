#include "Bank.h"
#include "Benefector.h"
#include "Nosy.h"
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;

int main()
{
	cout<<"main thread started"<<endl;
	vector <int> ids;
	ids.push_back(1);
	ids.push_back(2);
	ids.push_back(3);
	ids.push_back(4);
	ids.push_back(5);

	Bank bank;
	BenefectorGenerator benefectors(&bank);
	NosyGenerator nosies(&bank);

	bank.newAccount("ali","09122222222");
	bank.newAccount("hassan","09111111111");
	bank.newAccount("ramtin","02161114918");
	bank.newAccount("hadis","09357172121");
	bank.newAccount("mahan","09371937085");

	Account* ali_account = bank.getAccountByID(1);
	for (unsigned i = 0 ; i < 100000 ; i++)
		ali_account->IncAndReturn(1);

	nosies.generateNewNosy("fozul1",ids);
	nosies.generateNewNosy("fozul2",ids);
	nosies.generateNewNosy("fozul3",ids);
	nosies.generateNewNosy("fozul4",ids);
	nosies.generateNewNosy("fozul5",ids);
	nosies.generateNewNosy("fozul6",ids);
	nosies.generateNewNosy("fozul7",ids);
	nosies.generateNewNosy("fozul8",ids);
	nosies.generateNewNosy("fozul9",ids);
	nosies.generateNewNosy("fozul10",ids);

	benefectors.generateNewBenefector("billgates",ids);
	benefectors.generateNewBenefector("dianna",ids);
	


	usleep(100000);
	benefectors.Close();
	nosies.Close();
}
