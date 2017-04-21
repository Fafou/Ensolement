

CC=g++
CFLAGS=-std=c++11 -Wall -Wextra
LDFLAGS=

all: CFLAGS += -O2
all: bin/ensolement

debug: CFLAGS += -g
debug: bin/ensolement

bin/ensolement: build/main.o build/parseur.o
	$(CC) -o $@ $^ $(LDFLAGS)

build/main.o: src/main.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

build/parseur.o: src/configuration/parseur.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf build/*

cleanAll: clean
	rm -rf bin/*