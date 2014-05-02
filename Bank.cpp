#include "Bank.h"

Bank::Bank(void)
{
}

Bank::~Bank(void)
{
	Close();
}

void Bank::Close(void)
{
	for (unsigned i = 0 ; i < accs.size() ; i++)
		delete (accs[i]);
}

Account* Bank::newAccount(const string &name, const string &phoneNumber)
{
	pthread_mutex_t runningMutex;
	pthread_t runningThread;
	pthread_mutex_init(&runningMutex, 0);
	Account* result = new Account(name, phoneNumber, accs.size()+1, runningThread, runningMutex);
	accs.push_back(result);
	idAccsMap[result->getID()]=result;
	if(pthread_create(&runningThread, 0, RunAnAccount, (void*)result))
		throw AccountConstructEx();
	return result;
}

vector<Account*> Bank::getAccountsByIDs(const vector<int>& IDs)
{
	vector<Account*> result;
	for(unsigned i=0; i<IDs.size(); ++i)
		if(idAccsMap.find(IDs[i])!=idAccsMap.end())
			result.push_back(idAccsMap[IDs[i]]);
	return result;
}

Account* Bank::getAccountByID(int id)
{
	if(idAccsMap.find(id)==idAccsMap.end())
		return 0;
	return idAccsMap[id];
}

int AccountConstructEx::code(void) const
{
	return 1;
}

const char* AccountConstructEx::Declaration(void) const
{
	return "An error occured while creating a new account";
}
