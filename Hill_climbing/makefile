# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: hill

hill: main_hill.o Dictionary.o Dictionary_Tree.o Node.o
	$(CC) hill.o Dictionary.o Dictionary_Tree.o Node.o -o hill

main_hill.o: main_hill.cpp
	$(CC) $(CFLAGS) main_hill.cpp

Dictionary.o: Dictionary.cpp
	$(CC) $(CFLAGS) Dictionary.cpp

Dictionary_Tree.o: Dictionary_Tree.cpp
	$(CC) $(CFLAGS) Dictionary_Tree.cpp

Node.o: Node.cpp
	$(CC) $(CFLAGS) Node.cp

clean:
	rm -rf *o hill
