#ifndef __Moez_Hadis_Exception
#define __Moez_Hadis_Exception

class Exception
{
	public:
		virtual int code(void)=0;
		virtual const char *Declaration(void)=0;
};

#endif
