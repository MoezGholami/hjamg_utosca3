#include <iostream>
#include <string>
#include "Bank.h"
#include "Nosy.h"
#include "Benefector.h"
#include "TerminalCommands.h"
using namespace std;

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	Bank BankMelli;
	vector<Nosy*> Agents;
	vector<Benefector*> GoodMen;
	NosyGenerator NosyGen(&BankMelli);
	BenefectorGenerator BenGen(&BankMelli);
	string Command="";

	do
	{
		getline(cin, Command);
		DoCommand(Command, BankMelli, Agents, GoodMen, NosyGen, BenGen);
	}while(Command!=Command_Exit);

	for(unsigned i=0; i<Agents.size(); ++i)
		(Agents[i])->destruct();
	for(unsigned i=0; i<GoodMen.size(); ++i)
		(GoodMen[i])->destruct();
	BankMelli.Close();
	return 0;
}
