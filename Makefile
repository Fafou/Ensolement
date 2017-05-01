

CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -Isrc/ -Itest/
LDFLAGS=

all: CFLAGS += -O2
all: bin/ensolement

debug: CFLAGS += -g -DENSOLEMENT_VERBOSE
debug: bin/ensolement

test: CFLAGS += -g
test: bin/test

bin/test: build/mainTest.o build/test.o build/ensolement.o build/parseur.o
	$(CC) -o $@ $^ $(LDFLAGS)

bin/ensolement: build/main.o build/ensolement.o build/parseur.o
	$(CC) -o $@ $^ $(LDFLAGS)

build/ensolement.o: src/ensolement.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

build/parseur.o: src/configuration/parseur.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

build/test.o: test/test.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

build/main.o: src/main.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

build/mainTest.o: test/main.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf build/*

cleanAll: clean
	rm -rf bin/*