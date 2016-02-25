CC=g++
CFLAGS= -std=c++11 -O3 -Wall -Werror -Wextra -pedantic

all: jobs

jobs:
	$(CC) $(CFLAGS) main.cpp Process.cpp ReadyQueue.cpp DeviceQueues.cpp Sysgen.cpp -o jobs
clean:
	rm jobs main.o Process.o ReadyQueue.o DeviceQueues.o Sysgen.o
