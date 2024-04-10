ROOT=.
FLAGS=-std=c++17 -I ./toText/include -Wno-invalid-offsetof
OBJECTS=$(ROOT)/build/Storable.o $(ROOT)/build/Store.o

testBool: lib
	g++ $(FLAGS) -c $(ROOT)/example/testBool.cpp -o $(ROOT)/build/testBool.o 
	g++ $(OBJECTS) $(ROOT)/build/testBool.o -o $(ROOT)/build/testBool.exe

lib: $(OBJECTS)
	echo "finish build lib"

$(ROOT)/build/Storable.o: $(ROOT)/toText/src/Storable.cpp
	g++ $(FLAGS) -c $(ROOT)/toText/src/Storable.cpp -o $(ROOT)/build/Storable.o 

$(ROOT)/build/Store.o: $(ROOT)/toText/src/Store.cpp
	g++ $(FLAGS) -c $(ROOT)/toText/src/Store.cpp -o $(ROOT)/build/Store.o

clean:
	del /s "$(ROOT)/build"
