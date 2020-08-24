# Makefile comments
PROGRAMS = driver
CC = gcc
CFLAGS = -Wall -g
all: $(PROGRAMS)
clean:
	rm -f *.o

# C compilations
queue.o: queue.c queue.h car.h
	$(CC) $(CFLAGS) -c queue.c
priorityQueue.o: priorityQueue.c priorityQueue.h event.h queue.h
	$(CC) $(CFLAGS) -c priorityQueue.c

hashTable.o: hashTable.c hashTable.h
	$(CC) $(CFLAGS) -c hashTable.c
equalsInt.o: equalsInt.c equalsInt.h
	$(CC) $(CFLAGS) -c equalsInt.c
graph.o: graph.c graph.h equalsInt.h priorityQueue.h hashTable.h
	$(CC) $(CFLAGS) -c graph.c

trafficSimulator.o: trafficSimulator.c graph.h equalsInt.h trafficSimulator.h
	$(CC) $(CFLAGS) -c trafficSimulator.c
road.o: road.c car.h queue.h
	$(CC) $(CFLAGS) -c road.c
driver.o: driver.c trafficSimulator.h
	$(CC) $(CFLAGS) -c driver.c

# Executable programs
driver: driver.o trafficSimulator.o graph.o hashTable.o equalsInt.o queue.o priorityQueue.o road.o
	$(CC) $(CFLAGS) -o driver driver.o trafficSimulator.o graph.o hashTable.o equalsInt.o queue.o priorityQueue.o road.o

