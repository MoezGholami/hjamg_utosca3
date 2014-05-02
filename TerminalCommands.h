#ifndef __Moez_Hadis_TerminalCommands
#define __Moez_Hadis_TerminalCommands

#include <string>
#include <sstream>
#include <iostream>
#include "Bank.h"
#include "Benefector.h"
using namespace std;

class Nosy;
class NosyGenerator;

const string Command_Exit		=	"exit";

const string Command_AddAccount		=	"addAccount";
const string Command_ChangeAccVal	=	"addToAccount";
const string Command_CheckVal		=	"CheckAccount";
const string Command_AddAgent		=	"AddAgent";
const string Command_AddBenefector	=	"AddGoodMan";

void DoCommand(const string &query, Bank &bank, vector<Nosy*>& Agents, vector<Benefector*>&GoodMen,
		NosyGenerator &NosyGen, BenefectorGenerator &BenGen);
void AddAccount(istream &in, Bank &bank);
void AddValToAccount(istream &in, Bank &bank);
void CheckValue(istream &in, Bank &bank);
void AddAgenet(istream &in, vector<Nosy*> &Agents, NosyGenerator &NosyGen);
void AddBenefector(istream &in, vector<Benefector*> &GoodMen, BenefectorGenerator &BenGen);


bool isStringBlank(const string &s);

#endif
