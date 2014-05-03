LnkOpt		=	-Wall -O3
CmpOpt		=	$(LnkOpt) -c
FinalLnkOpt	=	-lpthread

Test_01.out: Test_01.o Benefector.o Nosy.o Account.o Bank.o
	g++  Test_01.o Account.o Benefector.o Nosy.o Bank.o $(FinalLnkOpt) -o Test_01.out
Test_01.o: Test_01.cpp Benefector.h Bank.h Nosy.h
	g++ $(CmpOpt) Test_01.cpp -o Test_01.o

Main.out: Main.o Benefector.o Nosy.o Account.o Bank.o TerminalCommands.o
	g++  Main.o Account.o Benefector.o Nosy.o Bank.o TerminalCommands.o $(FinalLnkOpt) -o Main.out
Main.o: Main.cpp Bank.h Nosy.h Benefector.h TerminalCommands.h
	g++ $(CmpOpt) Main.cpp -o Main.o

TerminalCommands.o: TerminalCommands.cpp TerminalCommands.h Nosy.h
	g++ $(CmpOpt)  TerminalCommands.cpp -o TerminalCommands.o

Bank.o: Bank.cpp Account.h Exception.h
	g++ $(CmpOpt)  Bank.cpp -o Bank.o

Benefector.o: Benefector.cpp Benefector.h Account.h
	g++ $(CmpOpt) Benefector.cpp -o Benefector.o

Nosy.o: Nosy.cpp Nosy.h Account.h Exception.h Bank.h
	g++ $(CmpOpt) Nosy.cpp -o Nosy.o

Account.o: Account.cpp Account.h Benefector.h Nosy.h 
	g++ $(CmpOpt) Account.cpp -o Account.o

Clean:
	rm -rv *.o *.out *.txt & reset && reset 
