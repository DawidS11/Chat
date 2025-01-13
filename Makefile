CC = g++

CFLAGS = -std=c++20 -Wall -Wextra $(CPPFLAGS)

#chat: client.o server.o
#	$(CC) $(CFLAGS) -o chat client.o server.o

client: client.o
	$(CC) $(CFLAGS) -o client client.o

server: server.o
	$(CC) $(CFLAGS) -o server server.o
	
client.o: client.cpp
	$(CC) $(CFLAGS) -c values.hpp client.cpp

server.o: server.cpp
	$(CC) $(CFLAGS) -c values.hpp server.cpp

clean:
	rm -f client server client.o server.o values.hpp.gch