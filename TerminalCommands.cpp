#include "TerminalCommands.h"
#include "Nosy.h"

void DoCommand(const string &query, Bank &bank, vector<Nosy*>& Agents, vector<Benefector*>&GoodMen,
		NosyGenerator &NosyGen, BenefectorGenerator &BenGen)
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
		AddAgenet(iss, Agents, NosyGen);
	else if(type==Command_AddBenefector);
		AddBenefector(iss, GoodMen, BenGen);
}

void AddAccount(istream &in, Bank &bank)
{
	string name, number;
	if(!(in>>name>>number))
		return ;
	bank.newAccount(name, number);
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

void AddAgenet(istream &in, vector<Nosy*> &Agents, NosyGenerator &NosyGen)
{
	string agname;
	int tid;
	vector<int> accIDs;
	if(!(in>>agname))
		return ;
	while(in>>tid)
		accIDs.push_back(tid);
	Nosy *created=NosyGen.newNosy(agname, accIDs);
	Agents.push_back(created);
}

void AddBenefector(istream &in, vector<Benefector*> &GoodMen, BenefectorGenerator &BenGen)
{
	string gname;
	int tid;
	vector<int> accIDs;
	if(!(in>>gname))
		return ;
	while(in>>tid)
		accIDs.push_back(tid);
	Benefector *created=BenGen.newBenefector(gname, accIDs);
	GoodMen.push_back(created);
}

bool isStringBlank(const string &s)
{
	for(unsigned i=0; i<s.size(); ++i)
		if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z') || (s[i]>='0' && s[i]<='9'))
			return true;
	return true;
}
