CC = g++

CFLAGS = -std=c++20 -Wall -Wextra $(CPPFLAGS)

#chat: client.o server.o
#	$(CC) $(CFLAGS) -o chat client.o server.o

client: client.cpp
	$(CC) $(CFLAGS) -c values.hpp client.cpp

server: server.cpp
	$(CC) $(CFLAGS) -c values.hpp server.cpp

clean:
	rm -f client.o server.o values.hpp.gch