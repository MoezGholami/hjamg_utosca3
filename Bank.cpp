#include "Bank.h"

AccountGenerator::AccountGenerator(void)
{
	idcounter=0;
}

Account AccountGenerator::newAccount(const string &name, const string &phoneNumber)
{
	pthread_mutex_t runningMutex;
	pthread_t runningThread;
	pthread_mutex_init(&runningMutex, 0);
	Account result=Account(name, phoneNumber, idcounter++, runningThread, runningMutex);
	if(pthread_create(&runningThread, 0, RunAnAccount, (void*)&result))
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
