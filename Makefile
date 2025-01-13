CC = g++

CFLAGS = -std=c++20 -Wall -Wextra $(CPPFLAGS)

#chat: client.o server.o
#	$(CC) $(CFLAGS) -o chat client.o server.o

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp

clean:
	rm -f client.o server.o