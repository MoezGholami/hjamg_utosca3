#ifndef __Moez_Hadis_Exception
#define __Moez_Hadis_Exception

class Exception
{
	public:
		virtual int code(void) const=0;
		virtual const char *Declaration(void) const=0;
};

#endif
