CC = g++

CFLAGS = -std=c++20 -Wall -Wextra -pthread

all:
	make client; make server

client: client.o
	$(CC) $(CFLAGS) -o client client.o

server: server.o
	$(CC) $(CFLAGS) -o server server.o
	
client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp

clean:
	rm -f client server client.o server.o