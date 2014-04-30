#include "Bank.h"

Bank::Bank(void)
{
}

Bank::~Bank()
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
	if(pthread_create(&runningThread, 0, RunAnAccount, (void*)result))
		throw AccountConstructEx();
	return result;
}

int AccountConstructEx::code(void)
{
	return 1;
}

const char* AccountConstructEx::Declaration(void)
{
	return "An error occured while creating a new account";
}
