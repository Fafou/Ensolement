

CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -O3 
CFLAGS_DEBUG=-std=c++11 -Wall -Wextra -g
LDFLAGS=
EXEC=ensolement

all: bin/$(EXEC)

debug: bin/$(EXEC)Debug

bin/ensolement: build/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

bin/ensolementDebug: build/mainDebug.o
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

build/%Debug.o: src/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS_DEBUG)

clean:
	rm -rf build/*

cleanAll: clean
	rm -rf $(EXEC)