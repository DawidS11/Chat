CC = g++

CFLAGS = -std=c++20 -Wall -Wextra $(CPPFLAGS)

chat: main.o
	$(CC) $(CFLAGS) -o chat main.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f chat main.o