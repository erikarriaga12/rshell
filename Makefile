CC= g++
CFLAGS = -Wall -Werror -ansi -pedantic

all: 
	$(CC) $(CFLAGS) rshellnoclass.cpp -o rshell

rshell: 
	$(CC) $(CFLAGS) main.cpp
clean:
	rm rshell
