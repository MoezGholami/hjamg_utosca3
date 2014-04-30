test.out: test.o Benefector.o Nosy.o Account.o Bank.o
	g++  test.o Account.o Benefector.o Nosy.o Bank.o -lpthread -o test.out
test.o: test.cpp Benefector.h Bank.h Nosy.h
	g++ -c test.cpp -o test.o
Bank.o: Bank.cpp Account.h Exception.h
	g++ -c Bank.cpp -o Bank.o
Benefector.o: Benefector.cpp Benefector.h Account.h
	g++ -c Benefector.cpp -o Benefector.o
Nosy.o: Nosy.cpp Nosy.h Account.h
	g++ -c Nosy.cpp -o Nosy.o
Account.o: Account.cpp Account.h Benefector.h Nosy.h 
	g++ -c Account.cpp -o Account.o
clean:
	rm *.o test.out
