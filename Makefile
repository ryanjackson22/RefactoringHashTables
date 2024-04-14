CC = cl
FLAGS = /std:c++17 /WX /EHsc
OBJECTS = test.obj main.obj 

assignment6: $(OBJECTS)
	$(CC) /Fe"assignment6" $(OBJECTS)

test.obj: src\test.cpp src\HashTable.h
	$(CC) $(FLAGS) /I lib\ -c src\test.cpp

main.obj: src\main.cpp
	$(CC) $(FLAGS) /I lib\ -c src\main.cpp

clean:
	del assignment6.exe *.obj
