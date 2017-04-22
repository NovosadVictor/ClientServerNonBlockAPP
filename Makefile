CC=g++
CFLAGS=-c

all: db

db: main.o phone.o date.o billing.o
	$(CC) main.o phone.o date.o billing.o -o db
main.o: main.cpp date.cpp phone.cpp billing.cpp
	$(CC) $(CFLAGS) main.cpp
phone.o: phone.h phone.cpp
	$(CC) $(CFLAGS) phone.cpp
date.o: date.h date.cpp
	$(CC) $(CFLAGS) date.cpp
billing.o: billing.h billing.cpp
	$(CC) $(CFLAGS) billing.cpp

clean:
	rm -f *.o db
