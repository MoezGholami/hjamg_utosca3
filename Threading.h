#ifndef __Moez_Hadis_Threading
#define __Moez_Hadis_Threading

#include <pthread.h>
#include <unistd.h>

#define Lock(m)		while(pthread_mutex_trylock(&(m)))
#define UnLock(m)	while(pthread_mutex_unlock(&(m)))

#endif
