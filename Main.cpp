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
	NosyGenerator NosyGen(&BankMelli);
	BenefectorGenerator BenGen(&BankMelli);
	string Command="";

	do
	{
		getline(cin, Command);
		DoCommand(Command, BankMelli, NosyGen, BenGen);
	}while(Command!=Command_Exit);

	NosyGen.Close();
	BenGen.Close();
	BankMelli.Close();
	return 0;
}
