#include "TerminalCommands.h"
#include "Nosy.h"

void DoCommand(const string &query, Bank &bank, NosyGenerator &NosyGen, BenefectorGenerator &BenGen)
{
	if(isStringBlank(query))
		return ;
	istringstream iss(query);
	string type;
	iss>>type;
	if(type==Command_AddAccount)
		AddAccount(iss, bank);
	else if(type==Command_ChangeAccVal)
		AddValToAccount(iss, bank);
	else if(type==Command_CheckVal)
		CheckValue(iss, bank);
	else if(type==Command_AddAgent)
		AddAgenet(iss, NosyGen);
	else if(type==Command_AddBenefector);
		AddBenefector(iss, BenGen);
}

void AddAccount(istream &in, Bank &bank)
{
	string name, number;
	if(!(in>>name>>number))
		return ;
	try
	{
		bank.newAccount(name, number);
	}
	catch(const Exception &e)
	{
		cerr<<"An exception with Code "<<e.Code()<<" has been caught:\n"<<e.Declaration()<<endl;
	}
}

void AddValToAccount(istream &in, Bank &bank)
{
	int id, amount;
	if(!(in>>id>>amount))
		return ;
	Account* acc=bank.getAccountByID(id);
	if(acc==0)
		return ;
	acc->IncAndReturn(amount);
}

void CheckValue(istream &in, Bank &bank)
{
	int id, val;
	if(!(in>>id))
		return ;
	Account* acc=bank.getAccountByID(id);
	if(acc==0)
		return ;
	val=acc->IncAndReturn(0);
	cerr<<"the value of account with ID "<<id<<" is =\t"<<val<<endl;
}

void AddAgenet(istream &in, NosyGenerator &NosyGen)
{
	string agname;
	int tid;
	vector<int> accIDs;
	if(!(in>>agname))
		return ;
	while(in>>tid)
		accIDs.push_back(tid);
	try
	{
		NosyGen.generateNewNosy(agname, accIDs);
	}
	catch(const Exception &e)
	{
		cerr<<"An exception with Code "<<e.Code()<<" has been caught:\n"<<e.Declaration()<<endl;
	}
}

void AddBenefector(istream &in, BenefectorGenerator &BenGen)
{
	string gname;
	int tid;
	vector<int> accIDs;
	if(!(in>>gname))
		return ;
	while(in>>tid)
		accIDs.push_back(tid);
	try
	{
		BenGen.generateNewBenefector(gname, accIDs);
	}
	catch(const Exception &e)
	{
		cerr<<"An exception with Code "<<e.Code()<<" has been caught:\n"<<e.Declaration()<<endl;
	}
}

bool isStringBlank(const string &s)
{
	for(unsigned i=0; i<s.size(); ++i)
		if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z') || (s[i]>='0' && s[i]<='9'))
			return true;
	return true;
}
