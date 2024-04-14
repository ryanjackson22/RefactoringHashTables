CC = g++
FLAGS = -std=c++17 -Wall -Werror -Wextra -Wpedantic
VPATH = src:lib
OBJECTS = test.o main.o 

assignment6: $(OBJECTS)
	$(CC) $(OBJECTS) -o assignment6

debug: FLAGS += -g
debug: assignment6

test.o: test.cpp HashTable.h
	$(CC) $(FLAGS) -Ilib -c src/test.cpp

main.o: main.cpp
	$(CC) $(FLAGS) -Ilib -c src/main.cpp

clean:
	rm assignment6 *.o
