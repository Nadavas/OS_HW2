CC = g++
CFLAGS = -Wall -g

main: main.o bank.o
	$(CC) $(CFLAGS) -o main main.o bank.o

main.o: main.cpp bank.h
	$(CC) $(CFLAGS) -c main.cpp

bank.o: bank.cpp bank.h
	$(CC) $(CFLAGS) -c bank.cpp

clean:
	rm -f main *.o