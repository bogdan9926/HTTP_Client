CC=g++ -std=c++11 -Wall -Wextra

all: build

build: client

helpers.o: helpers.cpp
	$(CC) $^ -c

buffer.o: buffer.cpp
	$(CC) $^ -c

requests.o: requests.cpp
	$(CC) $^ -c

client.o: client.cpp
	$(CC) $^ -c

client: requests.o helpers.o buffer.o client.o
	$(CC) $^ -o client


clean:
	rm *.o client